#include "precompiled_cpodf.h"
#include "hatchstyle.h"

#include <ostream>

namespace cpdoccore { 
namespace odf { 

hatch_style hatch_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"single")
        return hatch_style( single );
    else if (tmp == L"double")
        return hatch_style( doublee );
    else if (tmp == L"triple")
        return hatch_style(triple );

	else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return hatch_style(single);
    }
}
}
}
