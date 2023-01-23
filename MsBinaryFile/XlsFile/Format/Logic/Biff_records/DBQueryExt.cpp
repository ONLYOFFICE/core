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

#include "DBQueryExt.h"
#include "../Biff_structures/ConnGrbitDbtWeb.h"
#include "../Biff_structures/ConnGrbitDbtOledb.h"
#include "../Biff_structures/ConnGrbitDbtAdo.h"

namespace XLS
{

DBQueryExt::DBQueryExt()
{
}


DBQueryExt::~DBQueryExt()
{
}


BaseObjectPtr DBQueryExt::clone()
{
	return BaseObjectPtr(new DBQueryExt(*this));
}

void DBQueryExt::readFields(CFRecord& record)
{
	unsigned short	flags1, flags2;
	record >> frtHeaderOld >> dbt >> flags1;		
	
	fMaintain			= GETBIT(flags1, 0);
	fNewQuery			= GETBIT(flags1, 1);
	fImportXmlSource	= GETBIT(flags1, 2);
	fSPListSrc			= GETBIT(flags1, 3);
	fSPListReinitCache	= GETBIT(flags1, 4);
	fSrcIsXml			= GETBIT(flags1, 7);

	switch(dbt)
	{
		case 4:	grbitDbt.reset(new ConnGrbitDbtWeb);	break;
		case 5:	grbitDbt.reset(new ConnGrbitDbtOledb);	break;
		case 7:	grbitDbt.reset(new ConnGrbitDbtAdo);	break;
		default:
			record.skipNunBytes(2); break;	//unused
			
	}
	if (grbitDbt)
	{
		grbitDbt->load(record);
	}

	record >> flags2 >> bVerDbqueryEdit >> bVerDbqueryRefreshed >> bVerDbqueryRefreshableMin;
	
	fTxtWiz			= GETBIT(flags2, 0);
	fTableNames		= GETBIT(flags2, 1);

	record.skipNunBytes(3); //unused

	record >> coledb >> cstFuture >> wRefreshInterval >> wHtmlFmt >> cwParamFlags;

	PBT val1;
	for (unsigned short i = 0; i < cwParamFlags; i++)
	{
		record >> val1;
		rgPbt.push_back(val1);
	}

	char val2;
	for (unsigned short i = 0; i < cstFuture; i++)
	{
		record >> val2;
		rgbFutureBytes += val2;
	}
}

} // namespace XLS

