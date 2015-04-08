#include "../precompiled_cpodf.h"
#include "linemode.h"
#include "errors.h"

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const line_mode & _Val)
{
    switch(_Val.get_type())
    {
    case line_mode::Continuous:
        _Wostream << "continuous";
        break;
    case line_mode::SkipWhiteSpace:
        _Wostream << "skip-white-space";
        break;
    default:
        break;
    }
    return _Wostream;    
}

line_mode line_mode::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    
    if (tmp == L"continuous")
        return line_mode( Continuous );
    else if (tmp == L"skip-white-space")
        return line_mode( SkipWhiteSpace );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return line_mode( Continuous );
    }
}

}
}

