#include "../precompiled_cpodf.h"
#include "hatchstyle.h"

#include <ostream>

namespace cpdoccore { 
namespace odf { 
std::wostream & operator << (std::wostream & _Wostream, const hatch_style & _Val)
{
    switch(_Val.get_type())
    {
	case hatch_style::single:
        _Wostream << L"single";
        break;
	case hatch_style::doublee:
        _Wostream << L"double";
        break;
 	case hatch_style::triple:
        _Wostream << L"triple";
        break;
  }
    return _Wostream;
}
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
