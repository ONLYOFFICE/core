
#include "stylehorizontalpos.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const horizontal_pos & _Val)
{
    switch(_Val.get_type())
    {
    case horizontal_pos::Left:
        _Wostream << L"left";
        break;
    case horizontal_pos::Center:
        _Wostream << L"center";
        break;
    case horizontal_pos::Right:
        _Wostream << L"right";
        break;
    case horizontal_pos::FromLeft:
        _Wostream << L"from-left";
        break;
    case horizontal_pos::Inside:
        _Wostream << L"inside";
        break;
    case horizontal_pos::Outside:
        _Wostream << L"outside";
        break;
    case horizontal_pos::FromInside:
        _Wostream << L"from-inside";
        break;
    default:
        break;
    }
    return _Wostream;    
}

horizontal_pos horizontal_pos::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"left")
        return horizontal_pos( Left );
    else if (tmp == L"center")
        return horizontal_pos( Center );
    else if (tmp == L"right")
        return horizontal_pos( Right );
    else if (tmp == L"from-left")
        return horizontal_pos( FromLeft );
    else if (tmp == L"inside")
        return horizontal_pos( Inside );
    else if (tmp == L"outside")
        return horizontal_pos( Outside );
    else if (tmp == L"from-inside")
        return horizontal_pos( FromInside );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return horizontal_pos( Left );
    }
}

} }
