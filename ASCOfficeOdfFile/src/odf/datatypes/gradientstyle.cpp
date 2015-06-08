
#include "gradientstyle.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { 
namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const gradient_style & _Val)
{
    switch(_Val.get_type())
    {
	case gradient_style::linear:
        _Wostream << L"linear";
        break;
	case gradient_style::axial:
        _Wostream << L"axial";
        break;
 	case gradient_style::radial:
        _Wostream << L"radial";
        break;
  	case gradient_style::ellipsoid:
        _Wostream << L"ellipsoid";
        break;
  	case gradient_style::square:
        _Wostream << L"square";
        break;
  	case gradient_style::rectangular:
        _Wostream << L"rectangular";
        break;
  }
    return _Wostream;
}

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
