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

#include "sparklines.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const sparkline_type& _Val)
{
    switch(_Val.get_type())
    {
    case sparkline_type::Line:
        _Wostream << L"line";
        break;
    case sparkline_type::Column:
        _Wostream << L"column";
        break;
    case sparkline_type::Stacked:
        _Wostream << L"stacked";
        break;
        break;
    default:
        break;
    }
    return _Wostream;    
}
sparkline_type sparkline_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"line")
        return sparkline_type(Line);
    else if (tmp == L"column")
        return sparkline_type(Column);
    else if (tmp == L"stacked")
        return sparkline_type(Stacked);
    else
    {
        return sparkline_type(Line);
    }
}
//--------------------------------------------------------------------------------------------------
std::wostream& operator << (std::wostream& _Wostream, const sparkline_empty& _Val)
{
    switch (_Val.get_type())
    {
    case sparkline_empty::Span:
        _Wostream << L"span";
        break;
    case sparkline_empty::Gap:
        _Wostream << L"gap";
        break;
    case sparkline_empty::Zero:
        _Wostream << L"zero";
        break;
    default:
        break;
    }
    return _Wostream;
}
sparkline_empty sparkline_empty::parse(const std::wstring& Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"span")
        return sparkline_empty(Span);
    else if (tmp == L"gap")
        return sparkline_empty(Gap);
    else if (tmp == L"zero")
        return sparkline_empty(Zero);
    else
    {
        return sparkline_empty(Zero);
    }
}
//--------------------------------------------------------------------------------------------------
std::wostream& operator << (std::wostream& _Wostream, const sparkline_axis_type& _Val)
{
    switch (_Val.get_type())
    {
    case sparkline_axis_type::Individual:
        _Wostream << L"individual";
        break;
    case sparkline_axis_type::Group:
        _Wostream << L"group";
        break;
    case sparkline_axis_type::Custom:
        _Wostream << L"custom";
        break;
    default:
        break;
    }
    return _Wostream;
}
sparkline_axis_type sparkline_axis_type::parse(const std::wstring& Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"individual")
        return sparkline_axis_type(Individual);
    else if (tmp == L"group")
        return sparkline_axis_type(Group);
    else if (tmp == L"custom")
        return sparkline_axis_type(Custom);
    else
    {
        return sparkline_axis_type(Individual);
    }
}
} }
