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
   //     case subtitle:
			//res = L"subTitle";
			break;
		case graphic:
			res = L"body";
			break;
		case object:
			res = L"obj";
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
        case subtitle:
		case notes:
		case handout:
        case outline:
		case text:
			res = L"body";
			break;
		case page:
			res = L"sldImg";
			break;
	}
	return res;
}



} }
