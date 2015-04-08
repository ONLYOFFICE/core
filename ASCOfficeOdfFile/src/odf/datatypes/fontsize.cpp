#include "../precompiled_cpodf.h"
#include "fontsize.h"

#include <boost_string.h>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const font_size & _Val)
{
    switch(_Val.get_type())
    {
    case font_size::Length:
        _Wostream << _Val.get_length();
        break;
    case font_size::Percent:
        _Wostream << _Val.get_percent();
        break;
    default:
        break;
    }
    return _Wostream;
}

font_size font_size::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (boost::algorithm::contains(tmp, L"%"))
    {
        try 
        {
            return font_size( percent::parse(tmp) );
        }
        catch(errors::invalid_attribute &) {}
    }

    try 
    {
        return font_size( length::parse(tmp) );
    } 
    catch(errors::invalid_attribute & ) {}

    BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
    return font_size( percent(100.0) );
}

} }

