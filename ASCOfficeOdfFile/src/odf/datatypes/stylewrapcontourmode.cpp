
#include "stylewrapcontourmode.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const wrap_contour_mode & _Val)
{
    switch(_Val.get_type())
    {
    case wrap_contour_mode::Full:
        _Wostream << L"full";
        break;
    case wrap_contour_mode::Outside:
        _Wostream << L"outside";
        break;
    default:
        break;
    }
    return _Wostream;    
}

wrap_contour_mode wrap_contour_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"full")
        return wrap_contour_mode( Full );
    else if (tmp == L"outside")
        return wrap_contour_mode( Outside );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return wrap_contour_mode( Full );
    }
}

} }
