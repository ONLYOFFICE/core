#include "precompiled_cpodf.h"
#include "chartinterpolation.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

chart_interpolation chart_interpolation::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_interpolation( none );
    else if (tmp == L"cubic-spline")
        return chart_interpolation( cubicSpline);
    else if (tmp == L"b-spline")
        return chart_interpolation( bSpline );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_interpolation( none );
    }
}

} }
