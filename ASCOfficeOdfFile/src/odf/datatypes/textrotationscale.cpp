#include <boost/algorithm/string.hpp>

#include "textrotationscale.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_rotation_scale & _Val)
{
    switch(_Val.get_type())
    {
    case text_rotation_scale::Fixed:
        _Wostream << L"fixed";
        break;
    case text_rotation_scale::LineHeight:
        _Wostream << L"line-height";
        break;
    default:
        break;
    }
    return _Wostream;    
}

text_rotation_scale text_rotation_scale::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"fixed")
        return text_rotation_scale( Fixed );
    else if (tmp == L"line-height")
        return text_rotation_scale( LineHeight );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return text_rotation_scale( Fixed );
    }
}

} }
