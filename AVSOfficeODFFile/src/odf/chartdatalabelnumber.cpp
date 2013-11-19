#include "precompiled_cpodf.h"
#include "chartdatalabelnumber.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

chart_data_label_number chart_data_label_number::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return chart_data_label_number( none );
    else if (tmp == L"value")
        return chart_data_label_number( value );
    else if (tmp == L"percentage")
        return chart_data_label_number( percentage );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_data_label_number( none );
    }
}

} }
