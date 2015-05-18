#include <boost/algorithm/string.hpp>

#include "linetype.h"
#include "errors.h"

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const line_type & _Val)
{
    switch(_Val.get_type())
    {
    case line_type::None:
        _Wostream << "none";
        break;
    case line_type::Single:
        _Wostream << "single";
        break;
    case line_type::Double:
        _Wostream << "double";
        break;
    default:
        break;
    }
    return _Wostream;
}

line_type line_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    
    if (tmp == L"none")
        return line_type( None );
    else if (tmp == L"single")
        return line_type( Single );
    else if (tmp == L"double")
        return line_type( Double );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return None;
    }
}

} }

