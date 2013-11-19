#include "precompiled_cpodf.h"
#include "textautospace.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_autospace & _Val)
{
    switch(_Val.get_type())
    {
    case text_autospace::None:
        _Wostream << L"none";
        break;
    case text_autospace::IdeographAlpha:
        _Wostream << L"ideograph-alpha";
        break;
    default:
        break;
    }
    return _Wostream;    
}

text_autospace text_autospace::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return text_autospace( None );
    else if (tmp == L"ideograph-alpha")
        return text_autospace( IdeographAlpha );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return text_autospace( None );
    }
}

} }
