#include "precompiled_cpodf.h"
#include "chartsolidtype.h"

#include <ostream>

namespace cpdoccore { namespace odf {

chart_solid_type chart_solid_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"cuboid")
        return chart_solid_type( cuboid );
    else if (tmp == L"cylinder")
        return chart_solid_type( cylinder);
    else if (tmp == L"cone")
        return chart_solid_type( cone );
    else if (tmp == L"pyramid")
        return chart_solid_type( pyramid );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return chart_solid_type( cuboid );
    }
}



} }
