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

#include "chartlabelposition.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types {

std::wostream & operator << (std::wostream & _Wostream, const chart_label_position & _Val)
{
	switch(_Val.get_type())
	{
		case   chart_label_position::avoid_overlap:	_Wostream <<  L"avoid-overlap"; break;
		case   chart_label_position::bottom:		_Wostream <<  L"bottom";		break;
		case   chart_label_position::bottom_left:	_Wostream <<  L"bottom-left";	break;
		case   chart_label_position::bottom_right:	_Wostream <<  L"bottom_right";	break;
		case   chart_label_position::center:		_Wostream <<  L"center";		break;
		case   chart_label_position::inside:		_Wostream <<  L"insidev";		break;
		case   chart_label_position::left:			_Wostream <<  L"left";			break;
		case   chart_label_position::near_origin:	_Wostream <<  L"near-origin";	break;
		case   chart_label_position::outside:		_Wostream <<  L"outside";		break;
		case   chart_label_position::right:			_Wostream <<  L"right";			break;
		case   chart_label_position::top:			_Wostream <<  L"top";			break;
		case   chart_label_position::top_left:		_Wostream <<  L"top-left";		break;
		case   chart_label_position::top_right:		_Wostream <<  L"top-right";		break;
	}
    return _Wostream;    
}
chart_label_position chart_label_position::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"avoid-overlap")	return chart_label_position( avoid_overlap );
	else if (tmp == L"bottom")		return chart_label_position( bottom );
	else if (tmp == L"bottom-left")	return chart_label_position( bottom_left );
	else if (tmp == L"bottom-right")return chart_label_position( bottom_right );
	else if (tmp == L"center")		return chart_label_position( center );
	else if (tmp == L"inside")		return chart_label_position( inside );
	else if (tmp == L"left")		return chart_label_position( left );
	else if (tmp == L"near-origin")	return chart_label_position( near_origin );
	else if (tmp == L"outside")		return chart_label_position( outside );
	else if (tmp == L"right")       return chart_label_position( right );
	else if (tmp == L"top")			return chart_label_position( top );
	else if (tmp == L"top-left")	return chart_label_position( top_left );
	else if (tmp == L"top-right")	return chart_label_position( top_right );    
	else
    {
        return chart_label_position( near_origin );
    }
}



} }
