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
