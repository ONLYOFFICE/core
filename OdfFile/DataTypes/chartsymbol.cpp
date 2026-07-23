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

#include "chartsymbol.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const chart_symbol_type & _Val)
{
	switch(_Val.get_type())
	{
		case chart_symbol_type::noneSymbol:_Wostream << L"none"; break;
		case chart_symbol_type::autoSymbol:_Wostream << L"automatic"; break;
		case chart_symbol_type::namedSymbol:_Wostream << L"named-symbol"; break;
	}
    return _Wostream;    
}
std::wostream & operator << (std::wostream & _Wostream, const chart_symbol_name & _Val)
{
	switch(_Val.get_type())
	{
		case chart_symbol_name::noneSymbol:			_Wostream << L"none"; break;
		case chart_symbol_name::autoSymbol:			_Wostream << L"auto"; break;
		case chart_symbol_name::squareSymbol:		_Wostream << L"square"; break;
		case chart_symbol_name::diamondSymbol:		_Wostream << L"diamond"; break;
		case chart_symbol_name::arrow_downSymbol:	_Wostream << L"arrow-down"; break;
		case chart_symbol_name::arrow_upSymbol:		_Wostream << L"arrow-up"; break;
		case chart_symbol_name::arrow_rightSymbol:	_Wostream << L"arrow-right"; break;
		case chart_symbol_name::arrow_leftSymbol:	_Wostream << L"arrow-left"; break;
		case chart_symbol_name::bow_tieSymbol:		_Wostream << L"bow-tie"; break;
		case chart_symbol_name::hourglassSymbol:	_Wostream << L"hourglass"; break;
		case chart_symbol_name::circleSymbol:		_Wostream << L"circle"; break;
		case chart_symbol_name::starSymbol:			_Wostream << L"star"; break;
		case chart_symbol_name::xSymbol:			_Wostream << L"x"; break;
		case chart_symbol_name::plusSymbol:			_Wostream << L"plus"; break;
		case chart_symbol_name::asteriskSymbol:		_Wostream << L"asterisk"; break;
		case chart_symbol_name::horizontal_barSymbol:_Wostream << L"horizontal"; break;
		case chart_symbol_name::verticalSymbol:		_Wostream << L"vertical"; break;
	}
    return _Wostream;    
}

chart_symbol_type chart_symbol_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_symbol_type( noneSymbol );
    else if (tmp == L"auto" || tmp == L"automatic")
        return chart_symbol_type( autoSymbol );
    else if (tmp == L"named-symbol")
        return chart_symbol_type( namedSymbol );
    else
    {
        return chart_symbol_type( noneSymbol );
    }
}
chart_symbol_name chart_symbol_name::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if	(tmp == L"square")
        return chart_symbol_name( squareSymbol );
    else if (tmp == L"diamond")
		return chart_symbol_name( diamondSymbol);
    else if (tmp == L"arrow-down")
		return chart_symbol_name( arrow_downSymbol);
    else if (tmp == L"arrow-up")
		return chart_symbol_name( arrow_upSymbol);
    else if (tmp == L"arrow-right")
		return chart_symbol_name( arrow_rightSymbol);
    else if (tmp == L"arrow-left")
		return chart_symbol_name( arrow_leftSymbol);
    else if (tmp == L"bow-tie")
		return chart_symbol_name( bow_tieSymbol);
    else if (tmp == L"hourglass")
		return chart_symbol_name( hourglassSymbol);
    else if (tmp == L"circle")
		return chart_symbol_name( circleSymbol);
    else if (tmp == L"star")
		return chart_symbol_name( starSymbol);
    else if (tmp == L"x")
		return chart_symbol_name( xSymbol);
    else if (tmp == L"plus")
		return chart_symbol_name( plusSymbol);
    else if (tmp == L"asterisk")
		return chart_symbol_name( asteriskSymbol);
    else if (tmp == L"horizontal-bar" || tmp == L"horizontal")
		return chart_symbol_name( horizontal_barSymbol);
    else if (tmp == L"vertical-bar" || tmp == L"vertical")
		return chart_symbol_name( verticalSymbol);

    else
    {
        return chart_symbol_name( squareSymbol );
    }
}
} }
