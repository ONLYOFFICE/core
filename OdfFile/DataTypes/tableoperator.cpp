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

#include "tableoperator.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const table_operator& _Val)
{
    switch(_Val.get_type())
    {
    case table_operator::Equal:                 _Wostream << L"="; break;
    case table_operator::NotEqual:              _Wostream << L"!="; break;
    case table_operator::LessThan:              _Wostream << L"<"; break;
    case table_operator::GreaterThan:           _Wostream << L">"; break;
    case table_operator::LessThanOrEqual:       _Wostream << L"<="; break;
    case table_operator::GreaterThanOrEqual:    _Wostream << L">="; break;
    case table_operator::BeginsWith:            _Wostream << L"begins"; break;
    case table_operator::EndsWith:              _Wostream << L"ends"; break;
    case table_operator::Contains:              _Wostream << L"contains"; break;
    case table_operator::NotContains:           _Wostream << L"!contains"; break;
    case table_operator::NotBeginsWith:         _Wostream << L"!begins"; break;
    case table_operator::NotEndsWith:           _Wostream << L"!ends"; break;
    case table_operator::BottomPercent:         _Wostream << L"bottom percent"; break;
    case table_operator::BottomValues:          _Wostream << L"bottom values"; break;
    case table_operator::Empty:                 _Wostream << L"empty"; break;
    case table_operator::NonEmpty:              _Wostream << L"!empty"; break;
    case table_operator::TopPercent:            _Wostream << L"top percent"; break;
    case table_operator::TopValues:             _Wostream << L"top values"; break;
    case table_operator::UserDefined:           _Wostream << _Val.get_user_defined(); break;
    default:
        _Wostream << L"text";
        break;
    }
    return _Wostream;    
}

table_operator table_operator::parse(const std::wstring & Str)
{
    if (Str == L"=")
        return table_operator(Equal);
    else if (Str == L"!=")
        return table_operator(NotEqual);
    else if (Str == L"<")
        return table_operator(LessThan);
    else if (Str == L">")
        return table_operator(GreaterThan);
    else if (Str == L"<=")
        return table_operator(LessThanOrEqual);
    else if (Str == L">=")
        return table_operator(GreaterThanOrEqual);
    else if (Str == L"begins")
        return table_operator(BeginsWith);
    else if (Str == L"ends")
        return table_operator(EndsWith);
    else if (Str == L"!begins")
        return table_operator(NotBeginsWith);
    else if (Str == L"!ends")
        return table_operator(NotEndsWith);
    else if (Str == L"contains")
        return table_operator(Contains);
    else if (Str == L"!contains")
        return table_operator(NotContains);
    else if (Str == L"bottom percent")
        return table_operator(BottomPercent);
    else if (Str == L"bottom values")
        return table_operator(BottomValues);
    else if (Str == L"top percent")
        return table_operator(TopPercent);
    else if (Str == L"top values")
        return table_operator(TopValues);
    else if (Str == L"empty")
        return table_operator(Empty);
    else if (Str == L"!empty")
        return table_operator(NonEmpty);
    else
    {
        return table_operator(UserDefined, Str);
    }
}

} }
