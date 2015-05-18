
#include "fontpitch.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const font_pitch & _Val)
{
    switch(_Val.get_type())
    {
    case font_pitch::Fixed:
        _Wostream << L"fixed";
        break;
    case font_pitch::Variable:
        _Wostream << L"variable";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_pitch font_pitch::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"fixed")
        return font_pitch( Fixed );
    if (tmp == L"variable")
        return font_pitch( Variable );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return font_pitch( Fixed );
    }
}

} }

