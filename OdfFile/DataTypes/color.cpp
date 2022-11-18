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
				result = XmlUtils::GetUpper(tmp);
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

} }
