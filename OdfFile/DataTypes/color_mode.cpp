/*
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

#include "color_mode.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const color_mode& _Val)
{
    switch(_Val.get_type())
    {
    case color_mode::greyscale:
        _Wostream << L"greyscale";
        break;
    case color_mode::mono:
        _Wostream << L"mono";
        break;
    case color_mode::standard:
        _Wostream << L"standard";
        break;
    case color_mode::watermark:
        _Wostream << L"watermark";
        break;
    case color_mode::separating:
        _Wostream << L"separating";
        break;
    default:
        break;
    }
    return _Wostream;    
}
color_mode color_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"greyscale")
        return color_mode(greyscale);
    else if (tmp == L"mono")
        return color_mode(mono);
    else if (tmp == L"standard")
        return color_mode(standard);
    else if (tmp == L"watermark")
        return color_mode(watermark);
    else if (tmp == L"separating")
        return color_mode(separating);
    else
    {
        return color_mode(standard);
    }
}

} }
