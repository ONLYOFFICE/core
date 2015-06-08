
#include "rotationalign.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const rotation_align & _Val)
{
    switch(_Val.get_type())
    {
    case rotation_align::None:
        _Wostream << L"none";
        break;
    case rotation_align::Bottom:
        _Wostream << L"bottom";
        break;
    case rotation_align::Top:
        _Wostream << L"top";
        break;
    case rotation_align::Center:
        _Wostream << L"center";
        break;
    default:
        break;
    }
    return _Wostream;    
}

rotation_align rotation_align::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return rotation_align( None );
    else if (tmp == L"bottom")
        return rotation_align( Bottom );
    else if (tmp == L"top")
        return rotation_align( Top );
    else if (tmp == L"center")
        return rotation_align( Center );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return rotation_align( None );
    }
}

} }
