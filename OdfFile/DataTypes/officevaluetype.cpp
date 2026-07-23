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

#include "officevaluetype.h"
#include "../Common/errors.h"

#include <ostream>
#include <sstream>

namespace cpdoccore { namespace odf_types { 

std::wostream& operator << (std::wostream& _Wostream, const office_value_type& _Val)
{
    switch(_Val.get_type())
    {
    case office_value_type::Custom:
        //_Wostream << "none";
        break;
    case office_value_type::Float:
    case office_value_type::Scientific:
	case office_value_type::Fraction:
    {
        _Wostream << L"float";
    } break;
    case office_value_type::Currency:
        _Wostream << L"currency";
        break;
    case office_value_type::Percentage:
        _Wostream << L"percentage";
        break;
	case office_value_type::Date:
	case office_value_type::DateTime:
		_Wostream << L"date";
        break;	
 	case office_value_type::Time:
        _Wostream << L"time";
        break;	
	case office_value_type::Boolean:
        _Wostream << L"boolean";
        break;	
	case office_value_type::String:
    {
        _Wostream << L"string";
    }break;
    default:
        break;
    }
    return _Wostream;
}
bool operator == (office_value_type & t1, office_value_type::type & t2)
{
    if (t1.get_type() && t1.get_type()) return true;

    return false;
}
bool operator == (const office_value_type & t1, const office_value_type & t2)
{
    if (t1.get_type() && t1.get_type()) return true;

    return false;
}
bool operator == (office_value_type & t1, office_value_type & t2)
{
    if (t1.get_type() && t1.get_type()) return true;

    return false;
}
bool operator != (const office_value_type& t1, office_value_type::type& t2)
{
    if (t1.get_type() == office_value_type::Float)
    {
        if (t2 == office_value_type::Float || t2 == office_value_type::Scientific || t2 == office_value_type::Fraction) return false;
        return true;
    }
    else
    {
        return (t1.get_type() != t2);
    }
    return false;
}
office_value_type office_value_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    
    if (tmp == L"float")
        return office_value_type( Float);
    else if (tmp == L"currency")
        return office_value_type( Currency );
    else if (tmp == L"percentage")
        return office_value_type( Percentage );
	else if (tmp == L"boolean")
        return office_value_type( Boolean );
	else if (tmp == L"date")
        return office_value_type( Date );
	else if (tmp == L"time")
        return office_value_type( Time );
	else if (tmp == L"string")
        return office_value_type( String );
	else
		return office_value_type(Custom);
}

} }

