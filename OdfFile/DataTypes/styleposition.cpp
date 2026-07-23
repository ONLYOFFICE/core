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

#include <ostream>
#include <boost/algorithm/string.hpp>

#include "styleposition.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_position & _Val)
{
    switch(_Val.get_horisontal())
    {
    case style_position::Left:
        _Wostream << L"left ";
        break;
    case style_position::Center:
        _Wostream << L"center ";
        break;
    case style_position::Right:
        _Wostream << L"right ";
        break;
    default:
        break;
    }

    switch(_Val.get_vertical())
    {
    case style_position::Top:
        _Wostream << L"top";
        break;
    case style_position::Center:
        _Wostream << L"center";
        break;
    case style_position::Bottom:
        _Wostream << L"bottom";
        break;
    default:
        break;
    }

    return _Wostream;    
}

style_position style_position::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    type pos[2] = {Center, Center};
    int vertIndex = -1, horIndex = -1;

    for (size_t i = 0; i < (std::min)((size_t)2, splitted.size()); ++i)
    {
        if (splitted[i] == L"top" && -1 == vertIndex)
        {
            pos[1] = Top;
            vertIndex = (int)i;
        }
        else if (splitted[i] == L"bottom" && -1 == vertIndex)
        {
            pos[1] = Bottom;
            vertIndex = (int)i;
        }
        else if (splitted[i] == L"left" && -1 == horIndex)
        {
            pos[0] = Left;
            horIndex = (int)i;
        }
        else if (splitted[i] == L"right" && -1 == horIndex)
        {
            pos[0] = Right;
            horIndex = (int)i;
        }
        else if (splitted[i] == L"center")
        {}
        else
        {
            BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
        }
    }

    return style_position(pos[1], pos[0]);
}

} }
