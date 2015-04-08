#include "../precompiled_cpodf.h"
#include "chartregressiontype.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

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
