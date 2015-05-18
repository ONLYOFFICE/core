
#include "tablealign.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const table_align & _Val)
{
    switch(_Val.get_type())
    {
    case table_align::Left:
        _Wostream << L"left";
        break;
    case table_align::Center:
        _Wostream << L"center";
        break;
    case table_align::Right:
        _Wostream << L"right";
        break;
    case table_align::Margins:
        _Wostream << L"margins";
        break;
    default:
        break;
    }
    return _Wostream;    
}

table_align table_align::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"left")
        return table_align( Left );
    else if (tmp == L"center")
        return table_align( Center );
    else if (tmp == L"right")
        return table_align( Right );
    else if (tmp == L"margins")
        return table_align( Margins );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return table_align( Left );
    }
}

} }
