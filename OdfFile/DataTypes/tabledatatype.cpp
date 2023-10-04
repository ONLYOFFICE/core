﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
