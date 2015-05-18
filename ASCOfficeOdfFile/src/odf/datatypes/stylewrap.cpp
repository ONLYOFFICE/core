
#include "stylewrap.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const style_wrap & _Val)
{
    switch(_Val.get_type())
    {
    case style_wrap::None:
        _Wostream << L"none";
        break;
    case style_wrap::Left:
        _Wostream << L"left";
        break;
    case style_wrap::Right:
        _Wostream << L"right";
        break;
    case style_wrap::Parallel:
        _Wostream << L"parallel";
        break;
    case style_wrap::Dynamic:
        _Wostream << L"dynamic";
        break;
    case style_wrap::RunThrough:
        _Wostream << L"run-through";
        break;
    case style_wrap::Biggest:
        _Wostream << L"biggest";
        break;
    default:
        break;
    }
    return _Wostream;    
}

style_wrap style_wrap::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return style_wrap( None );
    else if (tmp == L"left")
        return style_wrap( Left );
    else if (tmp == L"right")
        return style_wrap( Right );
    else if (tmp == L"parallel")
        return style_wrap( Parallel );
    else if (tmp == L"dynamic")
        return style_wrap( Dynamic );
    else if (tmp == L"run-through")
        return style_wrap( RunThrough );
    else if (tmp == L"biggest")
        return style_wrap( Biggest );

    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return style_wrap( None );
    }
}

} }
