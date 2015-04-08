#include "../precompiled_cpodf.h"
#include "fontrelief.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const font_relief & _Val)
{
    switch(_Val.get_type())
    {
    case font_relief::None:
        _Wostream << L"none";
        break;
    case font_relief::Embossed:
        _Wostream << L"embossed";
        break;
    case font_relief::Engraved:
        _Wostream << L"engraved";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_relief font_relief::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return font_relief( None );
    else if (tmp == L"embossed")
        return font_relief( Embossed );
    else if (tmp == L"engraved")
        return font_relief( Engraved );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return font_relief( None );
    }
}

} }
