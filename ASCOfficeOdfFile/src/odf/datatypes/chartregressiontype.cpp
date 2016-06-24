/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "chartregressiontype.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const chart_regression_type & _Val)
{
	switch(_Val.get_type())
	{
	case   chart_regression_type::none: _Wostream <<  L"none"; break;
	case   chart_regression_type::linear: _Wostream <<  L"linear"; break;
	case   chart_regression_type::logarithmic : _Wostream <<  L"logarithmic"; break;
	case   chart_regression_type::exponential : _Wostream <<  L"exponential"; break;
	case   chart_regression_type::power : _Wostream <<  L"power"; break;
	}
    return _Wostream;    
}

chart_regression_type chart_regression_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_regression_type( none );
    else if (tmp == L"linear")
        return chart_regression_type( linear );
    else if (tmp == L"logarithmic")
        return chart_regression_type( logarithmic );
    else if (tmp == L"exponential")
        return chart_regression_type( exponential );
    else if (tmp == L"power")
        return chart_regression_type( power );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_regression_type( none );
    }
}

} }
