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
#include <boost/algorithm/string.hpp>

#include "chartclass.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 
std::wostream & operator << (std::wostream & _Wostream, const chart_class & _Val)
{
	std::wstring res = L"";
	switch(_Val.get_type())
	{
        case chart_class::area:			_Wostream << L"chart:area";		break;
        case chart_class::bubble:		_Wostream << L"chart:bubble";	break;
		case chart_class::circle:		_Wostream << L"chart:circle";	break;
		case chart_class::filled_radar:	_Wostream << L"chart:filled-radar";		break;
		case chart_class::gantt:		_Wostream << L"chart:gantt";	break;
		case chart_class::line:			_Wostream << L"chart:line";		break;
		case chart_class::radar:		_Wostream << L"chart:radar";	break;
		case chart_class::ring:			_Wostream << L"chart:ring";		break;
		case chart_class::scatter:		_Wostream << L"chart:scatter";	break;
		case chart_class::stock:		_Wostream << L"chart:stock";	break;
		case chart_class::surface:		_Wostream << L"chart:surface";	break;
		case chart_class::bar:
		default:						_Wostream << L"chart:bar";
	}
    return _Wostream;
}
chart_class chart_class::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

		 if (tmp == L"chart:area")			return chart_class(area);
    else if (tmp == L"chart:bar")			return chart_class(bar);
    else if (tmp == L"chart:bubble")		return chart_class(bubble);
    else if (tmp == L"chart:circle")		return chart_class(circle);
	else if (tmp == L"chart:filled-radar")	return chart_class(filled_radar);
    else if (tmp == L"chart:gantt")			return chart_class(gantt);
    else if (tmp == L"chart:line")			return chart_class(line);
    else if (tmp == L"chart:radar")			return chart_class(radar);
    else if (tmp == L"chart:ring")			return chart_class(ring);
    else if (tmp == L"chart:scatter")		return chart_class(scatter);
    else if (tmp == L"chart:stock")			return chart_class(stock);
    else if (tmp == L"chart:surface")		return chart_class(surface);
	else
    {
        return chart_class(bar);
    }
}




} }
