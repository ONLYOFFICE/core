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
#if USE_MAP
	smil_attribute_name::attibute_name_map smil_attribute_name::attribute_name_mapping_ = {
		{ L"charColor"		, smil_attribute_name::charColor },
		{ L"charFontName"	, smil_attribute_name::charFontName },
		{ L"charHeight"		, smil_attribute_name::charHeight },
		{ L"charPosture"	, smil_attribute_name::charPosture },
		{ L"charUnderline"	, smil_attribute_name::charUnderline },
		{ L"charWeight"		, smil_attribute_name::charWeight },
		{ L"color"			, smil_attribute_name::color },
		{ L"fill"			, smil_attribute_name::fill},
		{ L"fillColor"		, smil_attribute_name::fillColor },
		{ L"fillStyle"		, smil_attribute_name::fillStyle },
		{ L"height"			, smil_attribute_name::height },
		{ L"lineColor"		, smil_attribute_name::lineColor },
		{ L"lineStyle"		, smil_attribute_name::lineStyle },
		{ L"opacity"		, smil_attribute_name::opacity },
		{ L"rotate"			, smil_attribute_name::rotate },
		{ L"skewX"			, smil_attribute_name::skewX },
		{ L"skewY"			, smil_attribute_name::skewY },
		{ L"visibility"		, smil_attribute_name::visibility },
		{ L"width"			, smil_attribute_name::width },
		{ L"x"				, smil_attribute_name::x },
		{ L"y"				, smil_attribute_name::y },
			
		{ L"char-color"		, smil_attribute_name::charColor },
		{ L"char-font-name"	, smil_attribute_name::charFontName },
		{ L"char-height"	, smil_attribute_name::charHeight },
		{ L"char-posture"	, smil_attribute_name::charPosture },
		{ L"char-underline"	, smil_attribute_name::charUnderline },
		{ L"char-weight"	, smil_attribute_name::charWeight },
		{ L"fill-color"		, smil_attribute_name::fillColor },
		{ L"fill-style"		, smil_attribute_name::fillStyle },
		{ L"line-color"		, smil_attribute_name::lineColor },
		{ L"line-style"		, smil_attribute_name::lineStyle },
	};
#endif

	std::wostream& operator << (std::wostream& _Wostream, const smil_attribute_name& _Val)
	{
		switch (_Val.get_type())
		{
		case smil_attribute_name::charColor					: _Wostream << L"charColor"		; break;
		case smil_attribute_name::charFontName				: _Wostream << L"charFontName"	; break;
		case smil_attribute_name::charHeight				: _Wostream << L"charHeight"	; break;
		case smil_attribute_name::charPosture				: _Wostream << L"charPosture"	; break;
		case smil_attribute_name::charUnderline				: _Wostream << L"charUnderline"	; break;
		case smil_attribute_name::charWeight				: _Wostream << L"charWeight"	; break;
		case smil_attribute_name::color						: _Wostream << L"color"			; break;
		case smil_attribute_name::fill						: _Wostream << L"fill"; break;
		case smil_attribute_name::fillColor					: _Wostream << L"fillColor"		; break;
		case smil_attribute_name::fillStyle					: _Wostream << L"fillStyle"		; break;
		case smil_attribute_name::height					: _Wostream << L"height"		; break;
		case smil_attribute_name::lineColor					: _Wostream << L"lineColor"		; break;
		case smil_attribute_name::lineStyle					: _Wostream << L"lineStyle"		; break;
		case smil_attribute_name::opacity					: _Wostream << L"opacity"		; break;
		case smil_attribute_name::rotate					: _Wostream << L"rotate"		; break;
		case smil_attribute_name::skewX						: _Wostream << L"skewX"			; break;
		case smil_attribute_name::skewY						: _Wostream << L"skewY"			; break;
		case smil_attribute_name::visibility				: _Wostream << L"visibility"	; break;
		case smil_attribute_name::width						: _Wostream << L"width"			; break;
		case smil_attribute_name::x							: _Wostream << L"x"				; break;
		case smil_attribute_name::y							: _Wostream << L"y"				; break;
		}
		return _Wostream;
	}

	smil_attribute_name smil_attribute_name::parse(const std::wstring& Str)
	{
		smil_attribute_name result;
		const size_t repeats = 10000000;

#if USE_MAP

		attibute_name_map::const_iterator it = attribute_name_mapping_.find(Str);
		if (it == attribute_name_mapping_.end())
			result = smil_attribute_name::none;
		else
			result = it->second;

		return result;
#else

		if (Str == L"charColor")				result = smil_attribute_name(charColor);
		else if (Str == L"charFontName")		result = smil_attribute_name(charFontName);
		else if (Str == L"charHeight")			result = smil_attribute_name(charHeight);
		else if (Str == L"charPosture")			result = smil_attribute_name(charPosture);
		else if (Str == L"charUnderline")		result = smil_attribute_name(charUnderline);
		else if (Str == L"charWeight")			result = smil_attribute_name(charWeight);
		else if (Str == L"color")				result = smil_attribute_name(color);
		else if (Str == L"fillColor")			result = smil_attribute_name(fillColor);
		else if (Str == L"fillStyle")			result = smil_attribute_name(fillStyle);
		else if (Str == L"height")				result = smil_attribute_name(height);
		else if (Str == L"lineColor")			result = smil_attribute_name(lineColor);
		else if (Str == L"lineStyle")			result = smil_attribute_name(lineStyle);
		else if (Str == L"opacity")				result = smil_attribute_name(opacity);
		else if (Str == L"rotate")				result = smil_attribute_name(rotate);
		else if (Str == L"skewX")				result = smil_attribute_name(skewX);
		else if (Str == L"skewY")				result = smil_attribute_name(skewY);
		else if (Str == L"visibility")			result = smil_attribute_name(visibility);
		else if (Str == L"width")				result = smil_attribute_name(width);
		else if (Str == L"x")					result = smil_attribute_name(x);
		else if (Str == L"y")					result = smil_attribute_name(y);

		else if (Str == L"char-color")			result = smil_attribute_name(charColor);
		else if (Str == L"char-font-name")		result = smil_attribute_name(charFontName);
		else if (Str == L"char-height")			result = smil_attribute_name(charHeight);
		else if (Str == L"char-posture")		result = smil_attribute_name(charPosture);
		else if (Str == L"char-underline")		result = smil_attribute_name(charUnderline);
		else if (Str == L"char-weight")			result = smil_attribute_name(charWeight);
		else if (Str == L"fill-color")			result = smil_attribute_name(fillColor);
		else if (Str == L"fill-style")			result = smil_attribute_name(fillStyle);
		else if (Str == L"line-color")			result = smil_attribute_name(lineColor);
		else if (Str == L"line-style")			result = smil_attribute_name(lineStyle);

		return result;
#endif
	}

}
}
