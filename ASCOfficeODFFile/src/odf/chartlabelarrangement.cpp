#include "precompiled_cpodf.h"
#include "chartlabelarrangement.h"

#include <ostream>

namespace cpdoccore { namespace odf {

chart_label_arrangement chart_label_arrangement::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"side-by-side")
        return chart_label_arrangement( side_by_side );
    else if (tmp == L"stagger-even")
        return chart_label_arrangement( stagger_even);
    else if (tmp == L"stagger-odd")
        return chart_label_arrangement( stagger_odd );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_label_arrangement( side_by_side );
    }
}



} }
