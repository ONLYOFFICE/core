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

#include <string>
#include <cmath>

#include <boost/optional/optional_io.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

namespace cpdoccore { 

namespace {

struct color
{
    color(int r, int g, int b): r_(r), g_(g), b_(b) {}
    color(int r, int g, int b, std::wstring const & name): r_(r), g_(g), b_(b), name_(name) {}
    int r_,g_,b_;
    std::wstring name_;
};

color black(0,0,0, L"Black");
color green(0,255,0, L"Green");
color white(255,255,255, L"White");

color blue(0,0,255, L"Blue");
color magenta(255,0,255, L"Magenta");
color yellow(255,255,0, L"Yellow");

color cyan(0,0,0, L"Cyan");
color red(255,0,0, L"Red");

color colors[] = {black, green, white, blue, magenta, yellow, cyan, red };

template <class V> V sqr(V v){ return v*v; }

double color_dist(color const & c1, color const & c2)
{
    return sqrt((double)(sqr(c1.r_ - c2.r_) + sqr(c1.g_ - c2.g_) + sqr(c1.b_ - c2.b_)));
}

}

std::wstring RGBToString(int r, int g, int b)
{
    std::wstring result = L"";
    color v(r, g, b);
    double minDist = (std::numeric_limits<double>::max)();
    
    for (size_t i = 0; i < 6; i++)
    {
        double dist = color_dist(v, colors[i]);
        if (dist < minDist)
        {
            minDist = dist;
            result = colors[i].name_;
        }
    }

    return result;
}

template <class C>
bool hex2int(C i1, C i2, int & res)
{
    std::wstringstream s;    
    int res_ = 0;
    if ( (s << std::wstring(i1, i2)) &&  (s >> std::hex >> res_) && s.eof() )
    {
        res = res_;
        return true;
    }
    else
        return false;
}

bool HEXStringToRGB(std::wstring const & str, int &r, int &g, int &b)
{
    if (str.length() != 6)
        return false;

    try 
    {
        int r_, g_, b_;
        if (hex2int(str.begin(), str.begin()+2, r_) && 
            hex2int(str.begin()+2, str.begin()+4, g_) &&
            hex2int(str.begin()+4, str.begin()+6, b_)
            )
        {
            r = r_; g = g_; b = b_;
            return true;
        }
    }
    catch(...)
    {     
    }
    return false;
}

}
