#include "precompiled_cpodf.h"
#include "gradientstyle.h"

#include <ostream>

namespace cpdoccore { 
namespace odf { 

gradient_style gradient_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"linear")
        return gradient_style( linear );
    else if (tmp == L"axial")
        return gradient_style( axial );
    else if (tmp == L"radial")
        return gradient_style(radial );
    else if (tmp == L"ellipsoid")
        return gradient_style( ellipsoid);
    else if (tmp == L"square")
        return gradient_style( square );
    else if (tmp == L"rectangular")
        return gradient_style( rectangular );   
	else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return gradient_style( linear );
    }
}
}
}
