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

#include "color.h"

#include <ostream>
#include <sstream>

#include <boost/algorithm/string.hpp>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const color & _Color)
{
    _Wostream << _Color.get_color();
    return _Wostream;
}
bool operator== (const color & c1, const color & c2)
{
    if (c1.get_hex_value() == c2.get_hex_value())return true;
    else return false;
}

std::wstring html4_colors[16][2] = 
{	{L"aqua"	, L"#00ffff"},
	{L"black"	, L"#000000"}, 
	{L"blue"	, L"#0000ff"}, 
	{L"fuchsia"	, L"#ff00ff"}, 
	{L"gray"	, L"#808080"}, 
	{L"green"	, L"#008000"}, 
	{L"lime"	, L"#00ff00"}, 
	{L"maroon"	, L"#800000"}, 
	{L"navy"	, L"#000080"},
	{L"olive"	, L"#808000"}, 
	{L"purple"	, L"#800080"}, 
	{L"red"		, L"#ff0000"}, 
	{L"silver"	, L"#c0c0c0"}, 
	{L"teal"	, L"#008080"}, 
	{L"white"	, L"#FFFFFF"}, 
	{L"yellow"	, L"#ffff00"}
};
color color::parse(const std::wstring & Str)
{
	for (int i = 0 ; i < 16; i++)
	{
		if (Str == html4_colors[i][0])
			return color(html4_colors[i][1]);
	}
    return color(Str);
}

const std::wstring color::get_hex_value(bool alfa) const
{
    std::wstring tmp = color_;
	std::wstring result;
	if ( boost::algorithm::contains(tmp, L"#") )
    {
        boost::algorithm::trim(tmp);
        boost::algorithm::trim_left_if(tmp, boost::algorithm::is_any_of("#"));     

		result = XmlUtils::GetUpper(tmp);
    }
    else if (tmp.size() == 6)
    {
        try 
        {
            std::wstringstream s;
            unsigned int t = 0;
            if ((s << tmp) && (s >> std::hex >> t) && (s >> std::ws).eof())
            {
				result = tmp;
            }
        }
        catch(...)
        {
        }        
    }
	if (result.empty()) result = L"000000";
	if (alfa)	return L"FF" + result;
    else		return result;
}
//-----------------------------------------------------------------------------------------------------------------
std::wostream& operator << (std::wostream& _Wostream, const color_type& _Val)
{
    switch (_Val.get_type())
    {
    case color_type::rgb:
        _Wostream << L"rgb";
        break;
    case color_type::theme:
        _Wostream << L"theme";
        break;
    default:
        break;
    }
    return _Wostream;
}

color_type color_type::parse(const std::wstring& Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"rgb")
        return color_type(rgb);
    else if (tmp == L"theme")
        return color_type(theme);
    else
    {
        return color_type(rgb);
    }
}

} }
