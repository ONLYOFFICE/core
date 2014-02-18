#include "precompiled_cpodf.h"
#include "linebreak.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const line_break & _Val)
{
    switch(_Val.get_type())
    {
    case line_break::Normal:
        _Wostream << L"normal";
        break;
    case line_break::Strict:
        _Wostream << L"strict";
        break;
    default:
        break;
    }
    return _Wostream;    
}

line_break line_break::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"normal")
        return line_break( Normal );
    else if (tmp == L"strict")
        return line_break( Strict );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return line_break( Normal );
    }
}

} }
