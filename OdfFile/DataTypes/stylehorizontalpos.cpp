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

#include "stylehorizontalpos.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const horizontal_pos & _Val)
{
    switch(_Val.get_type())
    {
    case horizontal_pos::Left:
        _Wostream << L"left";
        break;
    case horizontal_pos::Center:
        _Wostream << L"center";
        break;
    case horizontal_pos::Right:
        _Wostream << L"right";
        break;
    case horizontal_pos::FromLeft:
        _Wostream << L"from-left";
        break;
    case horizontal_pos::Inside:
        _Wostream << L"inside";
        break;
    case horizontal_pos::Outside:
        _Wostream << L"outside";
        break;
    case horizontal_pos::FromInside:
        _Wostream << L"from-inside";
        break;
    default:
        break;
    }
    return _Wostream;    
}

horizontal_pos horizontal_pos::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"left")
        return horizontal_pos( Left );
    else if (tmp == L"center")
        return horizontal_pos( Center );
    else if (tmp == L"right")
        return horizontal_pos( Right );
    else if (tmp == L"from-left")
        return horizontal_pos( FromLeft );
    else if (tmp == L"inside")
        return horizontal_pos( Inside );
    else if (tmp == L"outside")
        return horizontal_pos( Outside );
    else if (tmp == L"from-inside")
        return horizontal_pos( FromInside );
    else
    {
        return horizontal_pos( Left );
    }
}

} }
