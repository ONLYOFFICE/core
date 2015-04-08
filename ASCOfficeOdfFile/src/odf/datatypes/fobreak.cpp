#include "../precompiled_cpodf.h"
#include "fobreak.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const fo_break & _Val)
{
    switch(_Val.get_type())
    {
    case fo_break::Auto:
        _Wostream << L"auto";
        break;
    case fo_break::Column:
        _Wostream << L"column";
        break;
    case fo_break::Page:
        _Wostream << L"page";
        break;
    default:
        break;
    }
    return _Wostream;    
}

fo_break fo_break::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"auto")
        return fo_break( Auto );
    else if (tmp == L"column")
        return fo_break( Column );
    else if (tmp == L"page")
        return fo_break( Page );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return fo_break( Auto );
    }
}

} }
