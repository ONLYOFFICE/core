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

#include "drawfill.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { 
namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const draw_fill & _Val)
{
	std::wstring fillType;
	switch(_Val.get_type())
	{
		case draw_fill::none:		_Wostream << L"none" ;break;
		case draw_fill::hatch:		_Wostream << L"hatch" ;break;
		case draw_fill::solid:		_Wostream << L"solid" ;break;
		case draw_fill::gradient:	_Wostream << L"gradient" ;break;
		case draw_fill::bitmap:		_Wostream << L"bitmap" ;break;
	}

    return _Wostream;
}
//std::wstring draw_fill::get_type_ms()
//{
//	std::wstring fillType;
//	switch(type_)
//	{
//		case draw_fill::none:		fillType = L"a:noFill";break;
//		case draw_fill::hatch:		fillType = L"a:pattFill";break;
//		case draw_fill::solid:		fillType = L"a:solidFill";break;
//		case draw_fill::gradient:	fillType = L"a:gradFill";break;
//		case draw_fill::bitmap:		fillType = L"a:imageFill";break;
//	}
//	return fillType;
//}
draw_fill draw_fill::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return draw_fill( none );
    else if (tmp == L"solid")
        return draw_fill( solid );
    else if (tmp == L"bitmap")
        return draw_fill( bitmap );
    else if (tmp == L"gradient")
        return draw_fill( gradient );
    else if (tmp == L"hatch")
        return draw_fill( hatch );
    else
    {
        return draw_fill( solid );
    }
}
}
}
