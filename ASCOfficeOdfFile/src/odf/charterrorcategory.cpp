#include "precompiled_cpodf.h"
#include "charterrorcategory.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

chart_error_category chart_error_category::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_error_category( none );
    else if (tmp == L"variance")
        return chart_error_category( variance );
    else if (tmp == L"standard-deviation")
        return chart_error_category( standard_deviation );
    else if (tmp == L"percentage")
        return chart_error_category( percentage );
    else if (tmp == L"error-margin")
        return chart_error_category( error_margin );
    else if (tmp == L"constant")
        return chart_error_category( constant );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_error_category( none );
    }
}

} }
