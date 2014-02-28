#include "precompiled_cpodf.h"
#include "wrapoption.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const wrap_option & _Val)
{
    switch(_Val.get_type())
    {
    case wrap_option::NoWrap:
        _Wostream << L"no-wrap";
        break;
    case wrap_option::Wrap:
        _Wostream << L"wrap";
        break;
    default:
        break;
    }
    return _Wostream;    
}

wrap_option wrap_option::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"no-wrap")
        return wrap_option( NoWrap );
    else if (tmp == L"wrap")
        return wrap_option( Wrap );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return wrap_option( NoWrap );
    }
}

} }
