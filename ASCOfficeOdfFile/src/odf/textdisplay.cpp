#include "precompiled_cpodf.h"
#include "textdisplay.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_display & _Val)
{
    switch(_Val.get_type())
    {
    case text_display::True:
        _Wostream << L"true";
        break;
    case text_display::None:
        _Wostream << L"none";
        break;
    case text_display::Condition:
        _Wostream << L"condition";
        break;
    default:
        break;
    }
    return _Wostream;    
}

text_display text_display::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"true")
        return text_display( True );
    else if (tmp == L"none")
        return text_display( None );
    else if (tmp == L"condition")
        return text_display( Condition );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return text_display( True );
    }
}

} }
