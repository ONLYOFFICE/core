/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

void DbOrParamQry::writeFields(CFRecord& record)
{
    unsigned short	flags = 0;
    if (typeRecord == 2)
    {
        SETBITS(flags, 0, 2, query.dbt)
        SETBIT(flags, 3, query.fOdbcConn)
        SETBIT(flags, 4, query.fSql)
        SETBIT(flags, 5, query.fSqlSav)
        SETBIT(flags, 6, query.fWeb)
        SETBIT(flags, 7, query.fSavePwd)
        SETBIT(flags, 8, query.fTablesOnlyHTML)

        record << flags << query.cparams << query.cstQuery << query.cstWebPost << query.cstSQLSav << query.cstOdbcConn;
    }
    else
    {
        SETBITS(flags, 0, 1, param.pbt)
        SETBIT(flags, 2, param.fNonDefaultName)
        record << param.wTypeSql << flags << param.grbit << param.fVal;
    }
}

} // namespace XLS

