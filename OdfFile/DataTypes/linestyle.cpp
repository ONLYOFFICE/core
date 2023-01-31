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
#include <boost/algorithm/string.hpp>

#include "linestyle.h"
#include "../Common/errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const line_style & _Val)
{
    switch(_Val.get_type())
    {
    case line_style::None:
        _Wostream << "none";
        break;
    case line_style::Solid:
        _Wostream << "solid";
        break;
    case line_style::Dotted:
        _Wostream << "dotted";
        break;
    case line_style::Dash:
        _Wostream << "dash";
        break;
    case line_style::LongDash:
        _Wostream << "long-dash";
        break;
    case line_style::DotDash:
        _Wostream << "dot-dash";
        break;
    case line_style::DotDotDash:
        _Wostream << "dot-dot-dash";
        break;
    case line_style::Wave:
        _Wostream << "wave";
        break;
    default:
        break;
    }
    return _Wostream;
}

line_style line_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return line_style( None );
    else if (tmp == L"solid")
        return line_style( Solid );
    else if (tmp == L"dotted")
        return line_style( Dotted );
    else if (tmp == L"dash")
        return line_style( Dash );
    else if (tmp == L"long-dash")
        return line_style( LongDash );
    else if (tmp == L"dot-dash")
        return line_style( DotDash );
    else if (tmp == L"dot-dot-dash")
        return line_style( DotDotDash );
    else if (tmp == L"wave")
        return line_style( Wave );
    else
    {
        return None;
    }
}

} }

