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

#include "xlsx_utils.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>


#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore {

namespace oox {

bool IsNumber(const std::wstring &value)
{
	boost::wregex rule(L"^\\-{0,1}[0-9]*[.,]{0,1}[0-9]*$");
	boost::match_results<std::wstring::const_iterator> results;

	return boost::regex_search(value/*.begin(), value.end(), results*/, rule);
}
std::wstring GetNumberFromString(const std::wstring &value)
{
	boost::wregex rule(L"^\\s*\\-{0,1}[0-9]*[.,]{0,1}[0-9]*\\s*$");
	boost::match_results<std::wstring::const_iterator> results;

    if (boost::regex_match(value, results, rule))
    {
        return (results[0].str());
	}
	else
		return L"";
}
std::wstring getColAddress(size_t col)
{
    static const size_t r = (L'Z' - L'A' + 1);
    std::wstring res;
    size_t r0 = col / r;

    if (r0 > 0)
    {
        const std::wstring rest = getColAddress(col - r * r0);
        const std::wstring res	= getColAddress(r0-1) + rest;
        return res;
    }
    else
        return std::wstring(1, (wchar_t)(L'A' + col));
}

std::wstring getRowAddress(size_t row)
{
    return std::to_wstring(row + 1);
}

std::wstring getCellAddress(size_t col, size_t row, bool bAbsolute)
{
	std::wstring a = bAbsolute ? L"$" : L"";
	return a + getColAddress(col) + a + getRowAddress(row);
}

size_t getColAddressInv(const std::wstring & a_)
{
    std::wstring a = a_;
    boost::algorithm::to_upper(a);
    
	static const size_t r = (L'Z' - L'A' + 1);
    size_t mul = 1;
    bool f = true;
    size_t res = 0;

	for (int i = a.length() - 1; i >= 0; i--)
	{
		size_t v = a[i] - L'A';
        if (f)
            f = false;
        else
            v += 1;
        res += v * mul;
        mul *= r;
    }
    return res;
}

size_t getRowAdderssInv(const std::wstring & a_)
{
	int sz = a_.length();
	if (a_.length()>0)
	{
	   return boost::lexical_cast<size_t>(a_)-1;
	}
	else
		return 0;
}

bool splitCellAddress(const std::wstring & a_, std::wstring & col, std::wstring & row)
{   
	std::wstring a = a_;

    XmlUtils::replace_all( a, L"$", L"");
	
	boost::algorithm::to_upper(a);
	
	for (size_t i = 0; i < a.length(); i++)
	{
		if (a[i] >= L'0' && a[i] <= L'9')
		{
			if (col.empty()) return false;

			row += a[i];
		}
		else if (a[i] >= L'A' && a[i] <= L'Z')
		{
			if (false == row.empty()) return false;

			col += a[i];
		}
		else
			return false;
    }

	if (col.size() > 5 || row.size() > 7) return false;

	return true;
}

bool getCellAddressInv(const std::wstring & a_, size_t & col, size_t & row)
{
    std::wstring colStr = L"", rowStr = L"";
	
	if (false == splitCellAddress(a_, colStr, rowStr)) return false;
    
    col = getColAddressInv( colStr );
    row = getRowAdderssInv( rowStr );

	return true;
}

bool parseBoolVal(const std::wstring & str)
{
    std::wstring s = str;
    boost::algorithm::to_lower(s);
    
	if (L"false" == str || L"0" == str)
        return false;
    else
        return true;
}

std::wstring cellType2Str(XlsxCellType::type type)
{
    switch(type)
    {
	case XlsxCellType::d:
        return L"d";
	case XlsxCellType::b:
        return L"b";
    case XlsxCellType::n:
        return L"n";
    case XlsxCellType::e:
        return L"e";
    case XlsxCellType::s:
        return L"s";
    case XlsxCellType::str:
        return L"str";
    case XlsxCellType::inlineStr:
        return L"inlineStr";
    default:
        return L"";
    }
}

}
}
