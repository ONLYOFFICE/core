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

#include <sstream>

#include <boost/functional/hash/hash.hpp>

#include "xlsx_font.h"


namespace cpdoccore {
namespace oox {

void xlsx_serialize(std::wostream & _Wostream, const xlsx_color & color)
{
    return xlsx_serialize(_Wostream, color, L"color");
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_color & color, const std::wstring & nodeName)
{
    std::wstringstream strm;

    if (color.autoVal)
        strm << L"auto=\"" << (bool)(*color.autoVal) << L"\" ";

    if (color.indexed)
        strm << L"indexed=\"" << (*color.indexed) << L"\" ";

    if (color.rgb)
    {
        std::wstring rgb = *color.rgb;
        strm << L"rgb=\"" << rgb << L"\" ";
    }

    if (color.theme)
        strm << L"theme=\"" << (*color.theme) << L"\" ";

    if (color.tint)
        strm << L"tint=\"" << (*color.tint) << L"\" ";

    if (!strm.str().empty())
    {
        _Wostream << L"<" << nodeName <<L" ";
        _Wostream << strm.str();
        _Wostream << L"/>";
    }
}


bool xlsx_color::operator == (const xlsx_color & rVal) const
{
    const bool res = 
        autoVal == rVal.autoVal &&
        indexed == rVal.indexed &&
        rgb == rVal.rgb &&
        theme == rVal.theme &&
        tint == rVal.tint;
    return res;
}

bool xlsx_color::operator != (const xlsx_color & rVal) const
{
    return !(this->operator ==(rVal));
}

std::size_t hash_value(xlsx_color const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.autoVal.get_value_or(false));
    boost::hash_combine(seed, val.indexed.get_value_or(0));
    boost::hash_combine(seed, val.rgb.get_value_or(L""));
    boost::hash_combine(seed, val.theme.get_value_or(0));
    boost::hash_combine(seed, val.tint.get_value_or(0.0));
    return seed;    
}

}
}
