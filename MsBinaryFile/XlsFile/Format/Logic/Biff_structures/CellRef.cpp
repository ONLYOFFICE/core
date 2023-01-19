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

const std::wstring CellRef::toString() const
{
	if (to_string_cache.empty())
	{
		int row_norm = AUX::normalizeRow(row);
		int column_norm = AUX::normalizeColumn(column);

		if (0 == row_norm && 65535 == row_norm) // whole column or range of columns
		{
			row_norm = 1048575;
		}
		if (0 == column_norm && 255 == column_norm) // whole row or range of rows
		{
			column_norm = 16383;
		}
		return to_string_cache = AUX::loc2str(row_norm, rowRelative, column_norm, colRelative);
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

