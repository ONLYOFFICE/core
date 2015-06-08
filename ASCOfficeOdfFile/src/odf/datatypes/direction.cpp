
#include "direction.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const direction & _Val)
{
    switch(_Val.get_type())
    {
    case direction::Ltr:
        _Wostream << L"ltr";
        break;
    case direction::Ttb:
        _Wostream << L"ttb";
        break;
    default:
        break;
    }
    return _Wostream;    
}

direction direction::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"ltr")
        return direction( Ltr );
    else if (tmp == L"ttb")
        return direction( Ttb );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return direction( Ltr );
    }
}

} }
