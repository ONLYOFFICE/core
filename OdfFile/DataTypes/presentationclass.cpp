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

#include "presentationclass.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 
std::wostream & operator << (std::wostream & _Wostream, const presentation_class & _Val)
{
	std::wstring res = L"";
	switch(_Val.get_type())
	{
        case presentation_class::title:			_Wostream << L"title";		break;
        case presentation_class::subtitle:		_Wostream << L"subtitle";	break;
		case presentation_class::graphic:		_Wostream << L"graphic";	break;
		case presentation_class::object:		_Wostream << L"object";		break;
		case presentation_class::chart:			_Wostream << L"chart";		break;
		case presentation_class::table:			_Wostream << L"table";		break;
		case presentation_class::orgchart:		_Wostream << L"orgchart";	break;
		case presentation_class::header:		_Wostream << L"header";		break;
		case presentation_class::footer:		_Wostream << L"footer";		break;
		case presentation_class::date_time:		_Wostream << L"date-time";	break;
		case presentation_class::page_number:	_Wostream << L"page-number";break;
		case presentation_class::page:			_Wostream << L"page";		break;
		case presentation_class::notes:			_Wostream << L"notes";		break;
		case presentation_class::handout:		_Wostream << L"handout";	break;
        case presentation_class::outline:		_Wostream << L"outline";	break;
		case presentation_class::text:			_Wostream << L"text";		break;
		case presentation_class::body:			_Wostream << L"body";		break;
	}
    return _Wostream;
}
presentation_class presentation_class::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")				return presentation_class( page );
    else if (tmp == L"title")		return presentation_class( title);
    else if (tmp == L"outline")		return presentation_class( outline );
    else if (tmp == L"subtitle")	return presentation_class( subtitle);
	else if (tmp == L"text")		return presentation_class( text);
    else if (tmp == L"graphic")		return presentation_class( graphic );
    else if (tmp == L"object")		return presentation_class( object );
    else if (tmp == L"chart")		return presentation_class( chart );
    else if (tmp == L"table")		return presentation_class( table );
    else if (tmp == L"orgchart")	return presentation_class( orgchart );
    else if (tmp == L"notes")		return presentation_class( notes );
    else if (tmp == L"handout")		return presentation_class( handout );
    else if (tmp == L"header")		return presentation_class( header );
    else if (tmp == L"footer")		return presentation_class( footer );
    else if (tmp == L"date-time")	return presentation_class( date_time );
    else if (tmp == L"page-number")	return presentation_class( page_number );
	else if (tmp == L"body")		return presentation_class( body );
	else
    {
        return presentation_class( page );
    }
}

std::wstring presentation_class::get_type_ms()
{
	std::wstring res = L"";
	switch(type_)//str 2978
	{
        case title:
			res = L"title";
			break;
        case subtitle:
			res = L"body";
			break;
		case graphic:
			res = L"body";
			break;
		case object:
			res = L"body";
			break;
		case chart:
			res = L"chart";
			break;
		case table:
			res = L"tbl";
			break;
		case orgchart:
			res = L"chart";
			break;
		case header:
			res = L"hdr";
			break;
		case footer:
			res = L"ftr";
			break;
		case date_time:
			res = L"dt";
			break;
		case page_number:
			res = L"sldNum";
			break;
		case notes:
		case handout:
		case outline:
		case body:
		case text:
			res = L"body";
			break;
		case page:
			res = L"pic";
			break;
	}
	return res;
}



} }
