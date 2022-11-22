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

#include "fontstretch.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const font_stretch & _Val)
{
    switch(_Val.get_type())
    {
    case font_stretch::Normal:
        _Wostream << L"normal";
        break;
    case font_stretch::UltraCondensed:
        _Wostream << L"ultra-condensed";
        break;
    case font_stretch::ExtraCondensed:
        _Wostream << L"extra-condensed";
        break;
    case font_stretch::Condensed:
        _Wostream << L"condensed";
        break;
    case font_stretch::SemiCondensed:
        _Wostream << L"semi-condensed";
        break;
    case font_stretch::SemiExpanded:
        _Wostream << L"semi-expanded";
        break;
    case font_stretch::Expanded:
        _Wostream << L"expanded";
        break;
    case font_stretch::ExtraExpanded:
        _Wostream << L"extra-expanded";
        break;
    case font_stretch::UltraExpanded:
        _Wostream << L"ultra-expanded";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_stretch font_stretch::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"normal")
        return font_stretch( Normal );
    else if (tmp == L"ultra-condensed")
        return font_stretch( UltraCondensed );
    else if (tmp == L"extra-condensed")
        return font_stretch( ExtraCondensed );
    else if (tmp == L"condensed")
        return font_stretch( Condensed );
    else if (tmp == L"semi-condensed")
        return font_stretch( SemiCondensed );
    else if (tmp == L"semi-expanded")
        return font_stretch( SemiExpanded );
    else if (tmp == L"expanded")
        return font_stretch( Expanded );
    else if (tmp == L"extra-expanded")
        return font_stretch( ExtraExpanded );
    else if (tmp == L"ultra-expanded")
        return font_stretch( UltraExpanded );
    else
    {
        return font_stretch( Normal );
    }
}

} }
