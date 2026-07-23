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

#include "CellRef.h"
#include "CellRangeRef.h"

#include "../../Auxiliary/HelpFunc.h"


namespace XLS
{


CellRef::CellRef()
:	row(0),
	column(0),
	rowRelative(true),
	colRelative(true),
	fQuoted(false)
{
}



CellRef::CellRef(const std::wstring str_ref)
:	fQuoted(false)
{
	fromString(str_ref);
}


CellRef::CellRef(const int row_init, const int column_init, const bool row_relative_init, const bool col_relative_init)
:	row(row_init),
	column(column_init),
	rowRelative(row_relative_init),
	colRelative(col_relative_init),
	fQuoted(false)
{
}


BiffStructurePtr CellRef::clone()
{
	return BiffStructurePtr(new CellRef(*this));
}

const std::wstring CellRef::toString(const bool xlsb) const
{
	if (to_string_cache.empty())
	{	
		int maxRow = 0;
		int maxCol = 0;
		if(xlsb)
		{
			maxRow = 1048575;
			maxCol = 16383;
		}
		else
		{
			maxRow = 65535;
			maxCol = 255;
		}
        int row_norm = AUX::normalizeRow(row, xlsb);
        int column_norm = AUX::normalizeColumn(column, xlsb);

		if (0 == row_norm && maxRow == row_norm) // whole column or range of columns
		{
			row_norm = maxRow;
		}
		if (0 == column_norm && maxCol == column_norm) // whole row or range of rows
		{
			column_norm = maxCol;
		}
        return to_string_cache = AUX::loc2str(row_norm, rowRelative, column_norm, colRelative, xlsb);
	}
	return to_string_cache;

}


void CellRef::fromString(const std::wstring& str)
{
	AUX::str2loc(str, row, rowRelative, column, colRelative);
	to_string_cache.clear();
}



CellRef::operator std::wstring  () const
{
	return toString().c_str();
}


const int CellRef::getRow() const
{
	return row;
}
const int CellRef::getColumn() const
{
	return column;
}
const bool CellRef::getRowRelative() const
{
	return rowRelative;
}
const bool CellRef::getColumnRelative() const
{
	return colRelative;
}


void CellRef::operator+=(const CellRef& appended_ref)
{
	row += rowRelative ? appended_ref.getRow() : 0;
	column += colRelative ? appended_ref.getColumn() : 0;
	to_string_cache.clear();
}


void CellRef::operator-=(const CellRef& subtracted_ref)
{
	row -= rowRelative ? subtracted_ref.getRow() : 0;
	column -= colRelative ? subtracted_ref.getColumn() : 0;
	to_string_cache.clear();
}

bool operator==(const CellRef& ref1, const CellRef& ref2)
{
	return (ref1.getRow() == ref2.getRow() && ref1.getColumn() == ref2.getColumn());
}

} // namespace XLS

