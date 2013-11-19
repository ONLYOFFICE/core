#include "precompiled_cpodf.h"
#include "chartregressiontype.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

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
