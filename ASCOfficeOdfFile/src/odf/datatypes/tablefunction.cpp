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

#include "tablefunction.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const table_function & _Val)
{
    switch(_Val.get_type())
    {
    case table_function::Auto:
        _Wostream << L"auto";
        break;
    case table_function::average:
        _Wostream << L"average";
        break;
    case table_function::count:
        _Wostream << L"count";
        break;
    case table_function::countnums:
        _Wostream << L"countnums";
        break;
    case table_function::max:
        _Wostream << L"max";
        break;
    case table_function::min:
        _Wostream << L"min";
        break;
    case table_function::product:
        _Wostream << L"product";
        break;
    case table_function::stdev:
        _Wostream << L"stdev";
        break;
    case table_function::stdevp:
        _Wostream << L"stdevp";
        break;
    case table_function::sum:
        _Wostream << L"sum";
        break; 
	case table_function::var:
        _Wostream << L"var";
        break;   
	case table_function::varp:
        _Wostream << L"varp";
        break;   
	default:
        _Wostream << _Val.get_string();
        break;
    }
    return _Wostream;    
}
table_function table_function::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"auto")
        return table_function( Auto );
    else if (tmp == L"average")
        return table_function( average );
    else if (tmp == L"count")
        return table_function( count );
    else if (tmp == L"countnums")
        return table_function( countnums );
    else if (tmp == L"max")
        return table_function( max );
    else if (tmp == L"min")
        return table_function( min );
    else if (tmp == L"product")
        return table_function( product );
    else if (tmp == L"stdev")
        return table_function( stdev );
    else if (tmp == L"stdevp")
        return table_function( stdevp );
    else if (tmp == L"sum")
        return table_function( sum );
    else if (tmp == L"var")
        return table_function( var );
    else if (tmp == L"varp")
        return table_function( varp );
	else
    {
        return table_function( String, Str );
    }
}
} 
}
