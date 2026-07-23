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

#include "tabledatatype.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const table_data_type& _Val)
{
    switch(_Val.get_type())
    {
    case table_data_type::automatic:
        _Wostream << L"automatic";
        break;
    case table_data_type::text:
        _Wostream << L"text";
        break;
    case table_data_type::number:
        _Wostream << L"number";
        break;
    case table_data_type::background_color:
        _Wostream << L"background_color";
        break;
    case table_data_type::text_color:
        _Wostream << L"text_color";
        break;
    case table_data_type::user_defined:
        _Wostream << _Val.get_user_defined();
        break;
    default:
        _Wostream << L"text";
        break;
    }
    return _Wostream;    
}
table_data_type table_data_type::parse(const std::wstring & Str)
{
    if (Str == L"automatic")
        return table_data_type(automatic);
    else if (Str == L"text")
        return table_data_type(text);
    else if (Str == L"number")
        return table_data_type(number);
    else if (Str == L"background-color")
        return table_data_type(background_color);
    else if (Str == L"text-color")
        return table_data_type(text_color);
	else
    {
        return table_data_type(user_defined, Str);
    }
}

} }
