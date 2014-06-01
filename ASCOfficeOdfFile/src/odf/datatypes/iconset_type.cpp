#include "precompiled_cpodf.h"
#include "iconset_type.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const iconset_type & _Val)
{
    switch(_Val.get_type())
    {
    case iconset_type::Arrows3:
        _Wostream << L"3Arrows";
        break;
    case iconset_type::Arrows3Grey:
        _Wostream << L"3ArrowsGrey";
        break;
    case iconset_type::Flags3:
        _Wostream << L"3Flags";
        break;
    case iconset_type::Traffic3Lights1:
        _Wostream << L"3TrafficLights1";
        break;
     case iconset_type::Traffic3Lights2:
        _Wostream << L"3TrafficLights2";
        break;
   default:
        break;
    }
    return _Wostream;    
}

iconset_type iconset_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"3Arrows")
        return iconset_type( Arrows3 );
    else     if (tmp == L"3ArrowsGrey")
        return iconset_type( Arrows3Grey );
   else     if (tmp == L"3Flags")
        return iconset_type( Flags3 );
   else     if (tmp == L"3TrafficLights1")
        return iconset_type( Traffic3Lights1 );
    else     if (tmp == L"3TrafficLights2")
        return iconset_type( Traffic3Lights2 );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return iconset_type( Arrows3 );
    }
}

} }
