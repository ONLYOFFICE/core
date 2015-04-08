#include "../precompiled_cpodf.h"
#include "styletype.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const style_type & _Val)
{
    switch(_Val.get_type())
    {
    case style_type::Left:
        _Wostream << L"left";
        break;
    case style_type::Center:
        _Wostream << L"center";
        break;
    case style_type::Right:
        _Wostream << L"right";
        break;
    case style_type::Char:
        _Wostream << L"char";
        break;
    default:
        break;
    }
    return _Wostream;    
}

style_type style_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"left")
        return style_type( Left );
    else if (tmp == L"center")
        return style_type( Center );
    else if (tmp == L"right")
        return style_type( Right );
    else if (tmp == L"char")
        return style_type( Char );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return style_type( Left );
    }
}

} }
