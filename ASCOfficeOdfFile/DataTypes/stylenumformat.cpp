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

#include "stylenumformat.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const style_numformat & _Val)
{
    switch(_Val.get_type())
    {
    case style_numformat::arabic:
        _Wostream << L"1";
        break;
    case style_numformat::romanUc:
        _Wostream << L"I";
        break;
    case style_numformat::romanLc:
        _Wostream << L"i";
        break;
    case style_numformat::alphaUc:
        _Wostream << L"A";
        break;
    case style_numformat::alphaLc:
        _Wostream << L"a";
        break;
	case style_numformat::aiueo:
        _Wostream << L"ア, イ, ウ, ...";
        break;
    case style_numformat::chineseCounting:
        _Wostream << L"ｲ, ﾛ, ﾊ, ...";
        break;
    case style_numformat::chineseLegal:
        _Wostream << L"一, 二, 三, ...";
        break;
    case style_numformat::ideographLegal:
        _Wostream << L"壹, 貳, 參, ...";
        break;
    case style_numformat::ideographTraditional:
        _Wostream << L"甲, 乙, 丙, ...";
        break;
    case style_numformat::ideographZodiac:
        _Wostream << L"子, 丑, 寅, ...";
        break;
    case style_numformat::ideographZodiacTraditional:
        _Wostream << L"甲子, 乙丑, 丙寅, ...";
        break;
    case style_numformat::iroha:
        _Wostream << L"ｲ, ﾛ, ﾊ, ...";
        break;
    case style_numformat::koreanDigital:
        _Wostream << L"일, 이, 삼, ...";
        break;
    case style_numformat::russianLo:
        _Wostream << L"а, б, .., аа, аб, ... (ru)";
        break;
    case style_numformat::russianUp:
        _Wostream << L"А, Б, .., Аа, Аб, ... (ru)";
        break;
    default:
        break;
    }
    return _Wostream;    
}

style_numformat style_numformat::parse(const std::wstring & Str)
{
	if (Str.empty())
		return style_numformat( none );

    std::wstring tmp = Str;

    if (tmp == L"1")
        return style_numformat( arabic );
    else if (tmp == L"I")
        return style_numformat( romanUc );
    else if (tmp == L"i")
        return style_numformat( romanLc );
    else if (tmp == L"A")
        return style_numformat( alphaUc );
    else if (tmp == L"a")
        return style_numformat( alphaLc );
	else if (std::wstring::npos != std::wstring(L"А, Б, .., Аа, Аб, ... (ru)").find(tmp) ||
			 std::wstring::npos != std::wstring(L"А, Б, .., АА, ББ, ... (ru)").find(tmp))
        return style_numformat( russianUp );
	else if (std::wstring::npos != std::wstring(L"а, б, .., аа, аб, ... (ru)").find(tmp) ||
			 std::wstring::npos != std::wstring(L"а, б, .., аа, бб, ... (ru)").find(tmp))
		return style_numformat(russianLo);
	else if (tmp == L"일, 이, 삼, ...")
        return style_numformat( koreanDigital );
    else if (tmp == L"ｲ, ﾛ, ﾊ, ...")
        return style_numformat( iroha );
    else if (tmp == L"甲, 乙, 丙, ...")
        return style_numformat( ideographTraditional );
    else if (tmp == L"甲子, 乙丑, 丙寅, ...")
        return style_numformat( ideographZodiacTraditional );
    else if (tmp == L"子, 丑, 寅, ...")
        return style_numformat( ideographZodiac );
    else if (tmp == L"壹, 貳, 參, ...")
        return style_numformat( ideographLegal );
    else if (tmp == L"一, 二, 三, ...")
        return style_numformat( chineseLegal );
    else if (tmp == L"ｲ, ﾛ, ﾊ, ...")
        return style_numformat( chineseCounting );
    else if (tmp == L"ア, イ, ウ, ...")
        return style_numformat( aiueo );
	
	else
    {
        return style_numformat( arabic );
    }
}

} }
