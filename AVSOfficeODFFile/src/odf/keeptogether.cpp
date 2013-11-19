#include "precompiled_cpodf.h"
#include "keeptogether.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const keep_together & _Val)
{
    switch(_Val.get_type())
    {
    case keep_together::Auto:
        _Wostream << L"auto";
        break;
    case keep_together::Always:
        _Wostream << L"always";
        break;
    default:
        break;
    }
    return _Wostream;    
}

keep_together keep_together::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"auto")
        return keep_together( Auto );
    else if (tmp == L"always")
        return keep_together( Always );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return keep_together( Auto );
    }
}

} }
