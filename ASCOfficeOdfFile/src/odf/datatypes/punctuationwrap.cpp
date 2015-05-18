
#include "punctuationwrap.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const punctuation_wrap & _Val)
{
    switch(_Val.get_type())
    {
    case punctuation_wrap::Simple:
        _Wostream << L"simple";
        break;
    case punctuation_wrap::Hanging:
        _Wostream << L"hanging";
        break;
    default:
        break;
    }
    return _Wostream;    
}

punctuation_wrap punctuation_wrap::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"simple")
        return punctuation_wrap( Simple );
    else if (tmp == L"hanging")
        return punctuation_wrap( Hanging );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return punctuation_wrap( Simple );
    }
}

} }
