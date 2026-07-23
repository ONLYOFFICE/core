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