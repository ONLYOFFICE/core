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

#include "officevaluetype.h"
#include "../Common/errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const office_value_type & _Val)
{
    switch(_Val.get_type())
    {
    case office_value_type::Custom:
        //_Wostream << "none";
        break;
    case office_value_type::Float:
    case office_value_type::Scientific:
	case office_value_type::Fraction:
       _Wostream << "float";
        break;
    case office_value_type::Currency:
        _Wostream << "currency";
        break;
    case office_value_type::Percentage:
        _Wostream << "percentage";
        break;
	case office_value_type::Date:
	case office_value_type::DateTime:
		_Wostream << "date";
        break;	
 	case office_value_type::Time:
        _Wostream << "time";
        break;	
	case office_value_type::Boolean:
        _Wostream << "boolean";
        break;	
	case office_value_type::String:
        _Wostream << "string";
        break;	
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

