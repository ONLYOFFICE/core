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

#include "DbOrParamQry.h"

namespace XLS
{

DbOrParamQry::DbOrParamQry(int typeRecord_)
{
	typeRecord = typeRecord_;

	param.pbt = query.dbt = 0;
}

DbOrParamQry::~DbOrParamQry()
{
}


BaseObjectPtr DbOrParamQry::clone()
{
	return BaseObjectPtr(new DbOrParamQry(*this));
}

void DbOrParamQry::readFields(CFRecord& record)
{
	size_t size = record.getDataSize() - record.getRdPtr();

	if (typeRecord == 1)
	{
		unsigned short	flags;

		record >> param.wTypeSql >> flags >> param.grbit >> param.fVal;
		
		param.pbt				= GETBITS(flags, 0, 1);
		param.fNonDefaultName	= GETBIT(flags, 2);

	}
	else
	{
		unsigned short	flags;

		record >> flags >> query.cparams >> query.cstQuery >> query.cstWebPost >> query.cstSQLSav >> query.cstOdbcConn;	
			
		query.dbt				= GETBITS(flags, 0, 2);
		query.fOdbcConn			= GETBIT(flags, 3);
		query.fSql				= GETBIT(flags, 4);
		query.fSqlSav			= GETBIT(flags, 5);
		query.fWeb				= GETBIT(flags, 6);
		query.fSavePwd			= GETBIT(flags, 7);
		query.fTablesOnlyHTML	= GETBIT(flags, 8);	
	}
}

} // namespace XLS

