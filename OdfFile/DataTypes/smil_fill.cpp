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

#include "smil_fill.h"

#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf_types {

	smil_fill smil_fill::parse(const std::wstring& Str)
	{
		if (Str == L"remove") 					return smil_fill::type::_remove;
		else if (Str == L"freeze")				return smil_fill::type::_freeze;
		else if (Str == L"hold")				return smil_fill::type::_hold;
		else if (Str == L"transition")			return smil_fill::type::_transition;
		else if (Str == L"auto")				return smil_fill::type::_auto;
		else if (Str == L"default")				return smil_fill::type::_default;
		
		return smil_fill::type::none;
	}

	std::wostream& operator << (std::wostream& _Wostream, const smil_fill& _Val)
	{
		switch (_Val.get_type())
		{
		case smil_fill::type::_remove		: _Wostream << L"remove"	; break;
		case smil_fill::type::_freeze		: _Wostream << L"freeze"	; break;
		case smil_fill::type::_hold			: _Wostream << L"hold"		; break;
		case smil_fill::type::_transition	: _Wostream << L"transition"; break;
		case smil_fill::type::_auto			: _Wostream << L"auto"		; break;
		case smil_fill::type::_default		: _Wostream << L"default"	; break;
		}

		return _Wostream;
	}

} // namespace odf_types
} // namespace cpdoccore