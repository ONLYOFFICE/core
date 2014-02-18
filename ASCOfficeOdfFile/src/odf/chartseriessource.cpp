#include "precompiled_cpodf.h"
#include "chartseriessource.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

chart_series_source chart_series_source::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"columns")
        return chart_series_source( columns );
    else if (tmp == L"rows")
        return chart_series_source( rows );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_series_source( columns );
    }
}

} }
