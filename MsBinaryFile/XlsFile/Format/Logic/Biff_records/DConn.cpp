/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "DConn.h"
#include "../Biff_structures/DConnConnectionWeb.h"
#include "../Biff_structures/DConnConnectionOleDb.h"
#include "../Biff_structures/ConnGrbitDbtWeb.h"
#include "../Biff_structures/ConnGrbitDbtOledb.h"
#include "../Biff_structures/ConnGrbitDbtAdo.h"

namespace XLS
{

DConn::DConn()
{
}

DConn::~DConn()
{
}

BaseObjectPtr DConn::clone()
{
	return BaseObjectPtr(new DConn(*this));
}

void DConn::readFields(CFRecord& record)
{
	unsigned short flags1, reserved1;
	unsigned char flags2, reserved2, reserved3;
	
	record >> frtHeaderOld >> dbt >> flags1 >> cParams >> reserved1 >> flags2 >> reserved2;
	
	//DBT_ODBC		0x0001		ODBC-based source
	//DBT_DAO		0x0002		DAO-based source
	//DBT_WEB		0x0004		Web query
	//DBT_OLEDB		0x0005		OLE DB-based source
	//DBT_TXT		0x0006		Text-based source created via text query
	//DBT_ADO		0x0007		ADO record set	

	fSavePwd		=  GETBIT(flags1, 0);
	fTablesOnlyHtml	=  GETBIT(flags1, 1);
	fTableNames		=  GETBIT(flags1, 2);
	fDeleted		=  GETBIT(flags1, 3);
	fStandAlone		=  GETBIT(flags1, 4);
	fAlwaysUseConnectionFile =  GETBIT(flags1, 5);
	fBackgroundQuery=  GETBIT(flags1, 6);
	fRefreshOnLoad	=  GETBIT(flags1, 7);
	fSaveData		=  GETBIT(flags1, 8);
	
	fMaintain		=  GETBIT(flags1, 0);
	fNewQuery		=  GETBIT(flags1, 1);
	fImportXmlSource=  GETBIT(flags1, 2);
	fSPListSrc		=  GETBIT(flags1, 3);
	fSPListReinitCache	=  GETBIT(flags1, 4);
	fSrcIsXml		=  GETBIT(flags1, 7);

	switch (dbt)
	{
		case 4:	grbitDbt.reset(new ConnGrbitDbtWeb);	break;
		case 5:	grbitDbt.reset(new ConnGrbitDbtOledb);	break;
		case 7:	grbitDbt.reset(new ConnGrbitDbtAdo);	break;
		default:
			record.skipNunBytes(2); break;	//unused
	}
	if (grbitDbt)
		grbitDbt->load(record);

	record >> bVerDbqueryEdit >> bVerDbqueryRefreshed >> bVerDbqueryRefreshableMin >> wRefreshInterval >> wHtmlFmt >> rcc >> credMethod >> reserved3;
//wHtmlFmt
//0x0001 No formatting is applied
//0x0002 Rich text formatting only
//0x0003 Full HTML formatting, including cell formatting	
	if (dbt == 5)
	{
		record >> rgchSourceDataFile;
	}
	
	record >> rgchSourceConnectionFile >> rgchConnectionName >> rgchConnectionDesc;
	
	if (dbt == 1 || dbt == 5)
	{
		record >> rgchSSOApplicationID;
	}

	if (fTableNames)
	{
		record >> tableNames;
	}
	
	for ( unsigned short i = 0; fStandAlone && i < cParams; i++)
	{
		DConnParameter val;
		params.push_back(val);
	}

	switch (dbt)
	{
		case 1:	connection.reset(new XLUnicodeStringSegmented);	break;
		case 4:	connection.reset(new DConnConnectionWeb);		break;
		case 5:	connection.reset(new DConnConnectionOleDb);		break;
		case 6:	connection_txtQry.readFields(record);			break;
	}

	if (connection)
	{
		connection->load(record);
	}

	if (dbt == 1 || dbt == 5)
	{
		record >> rgbSQL;
	}
	
	if (dbt == 1 || dbt == 5)//7183958.xls
	{
		record >> rgbSQLSav;	
	}
	
	if (dbt == 4 || dbt == 5)
	{
		record >> rgbEditWebPage;
	}
	
	record >> id;

	int skip = record.getDataSize() - record.getRdPtr();

	record.skipNunBytes(skip);
}

} // namespace XLS

