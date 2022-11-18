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
