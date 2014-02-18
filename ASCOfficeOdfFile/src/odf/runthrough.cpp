#include "precompiled_cpodf.h"
#include "runthrough.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const run_through & _Val)
{
    switch(_Val.get_type())
    {
    case run_through::Foreground:
        _Wostream << L"foreground";
        break;
    case run_through::Background:
        _Wostream << L"background";
        break;
    default:
        break;
    }
    return _Wostream;    
}

run_through run_through::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"foreground")
        return run_through( Foreground );
    else if (tmp == L"background")
        return run_through( Background );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return run_through( Foreground );
    }
}

} }
