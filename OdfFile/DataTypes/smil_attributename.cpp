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

#include "smil_attributename.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types {

	std::wostream& operator << (std::wostream& _Wostream, const smil_attribute_name& _Val)
	{
		switch (_Val.get_type())
		{
		case smil_attribute_name::charColor					: _Wostream << L"char-color"	; break;
		case smil_attribute_name::charFontName				: _Wostream << L"char-font-name"; break;
		case smil_attribute_name::charHeight				: _Wostream << L"char-height"	; break;
		case smil_attribute_name::charPosture				: _Wostream << L"char-posture"	; break;
		case smil_attribute_name::charUnderline				: _Wostream << L"char-underline"; break;
		case smil_attribute_name::charWeight				: _Wostream << L"char-weight"	; break;
		case smil_attribute_name::color						: _Wostream << L"color"			; break;
		case smil_attribute_name::fill						: _Wostream << L"fill"			; break;
		case smil_attribute_name::fillColor					: _Wostream << L"fill-color"	; break;
		case smil_attribute_name::fillStyle					: _Wostream << L"fill-style"	; break;
		case smil_attribute_name::fillOn					: _Wostream << L"FillOn"		; break;
		case smil_attribute_name::height					: _Wostream << L"height"		; break;
		case smil_attribute_name::lineColor					: _Wostream << L"lineColor"		; break;
		case smil_attribute_name::lineStyle					: _Wostream << L"lineStyle"		; break;
		case smil_attribute_name::opacity					: _Wostream << L"opacity"		; break;
		case smil_attribute_name::stroke					: _Wostream << L"stroke"		; break;
		case smil_attribute_name::strokeColor				: _Wostream << L"stroke-color"	; break;
		case smil_attribute_name::rotate					: _Wostream << L"rotate"		; break;
		case smil_attribute_name::skewX						: _Wostream << L"skewX"			; break;
		case smil_attribute_name::skewY						: _Wostream << L"skewY"			; break;
		case smil_attribute_name::transform					: _Wostream << L"transform"		; break;
		case smil_attribute_name::visibility				: _Wostream << L"visibility"	; break;
		case smil_attribute_name::width						: _Wostream << L"width"			; break;
		case smil_attribute_name::x							: _Wostream << L"x"				; break;
		case smil_attribute_name::y							: _Wostream << L"y"				; break;
		case smil_attribute_name::dim						: _Wostream << L"dim"			; break;
		}
		return _Wostream;
	}

	smil_attribute_name smil_attribute_name::parse(const std::wstring& Str)
	{
		std::wstring tmp = boost::algorithm::to_lower_copy(Str);
		boost::algorithm::erase_all(tmp, L"-");

		if (tmp == L"charcolor")				return smil_attribute_name(charColor);
		else if (tmp == L"charfontname")		return smil_attribute_name(charFontName);
		else if (tmp == L"charheight")			return smil_attribute_name(charHeight);
		else if (tmp == L"charposture")			return smil_attribute_name(charPosture);
		else if (tmp == L"charunderline")		return smil_attribute_name(charUnderline);
		else if (tmp == L"charweight")			return smil_attribute_name(charWeight);
		else if (tmp == L"color")				return smil_attribute_name(color);
		else if (tmp == L"fill")				return smil_attribute_name(fill);
		else if (tmp == L"fillcolor")			return smil_attribute_name(fillColor);
		else if (tmp == L"fillstyle")			return smil_attribute_name(fillStyle);
		else if (tmp == L"fillon")				return smil_attribute_name(fillOn);
		else if (tmp == L"height")				return smil_attribute_name(height);
		else if (tmp == L"linecolor")			return smil_attribute_name(lineColor);
		else if (tmp == L"linestyle")			return smil_attribute_name(lineStyle);
		else if (tmp == L"opacity")				return smil_attribute_name(opacity);
		else if (tmp == L"stroke")				return smil_attribute_name(stroke);
		else if (tmp == L"strokecolor")			return smil_attribute_name(strokeColor);
		else if (tmp == L"rotate")				return smil_attribute_name(rotate);
		else if (tmp == L"skewx")				return smil_attribute_name(skewX);
		else if (tmp == L"skewx")				return smil_attribute_name(skewY);
		else if (tmp == L"transform")			return smil_attribute_name(transform);
		else if (tmp == L"visibility")			return smil_attribute_name(visibility);
		else if (tmp == L"width")				return smil_attribute_name(width);
		else if (tmp == L"x")					return smil_attribute_name(x);
		else if (tmp == L"y")					return smil_attribute_name(y);
		else if (tmp == L"dim")					return smil_attribute_name(dim);

		return smil_attribute_name(none);
	}

}
}
