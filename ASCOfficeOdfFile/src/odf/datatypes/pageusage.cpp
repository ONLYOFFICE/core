#include <boost/algorithm/string.hpp>

#include "pageusage.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const page_usage & _Val)
{
    switch(_Val.get_type())
    {
    case page_usage::All:
        _Wostream << L"all";
        break;
    case page_usage::Left:
        _Wostream << L"left";
        break;
    case page_usage::Right:
        _Wostream << L"right";
        break;
    case page_usage::Mirrored:
        _Wostream << L"mirrored";
        break;
    default:
        break;
    }
    return _Wostream;    
}

page_usage page_usage::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"all")
        return page_usage( All );
    else if (tmp == L"left")
        return page_usage( Left );
    else if (tmp == L"right")
        return page_usage( Right );
    else if (tmp == L"mirrored")
        return page_usage( Mirrored );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return page_usage( All );
    }
}

} }
