#include <boost/algorithm/string.hpp>

#include "lengthorpercent.h"
#include "errors.h"

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const length_or_percent & _Val)
{
    switch(_Val.get_type())
    {
    case length_or_percent::Percent:
        _Wostream << _Val.get_percent();
        break;
    case length_or_percent::Length:
        _Wostream << _Val.get_length();
        break;
    }
    return _Wostream;
}

length_or_percent length_or_percent::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (boost::algorithm::contains(tmp, L"%"))
    {
        try 
        {
            return length_or_percent( percent::parse(tmp) );
        }
        catch(errors::invalid_attribute &) {}
    }

    try 
    {
        return length_or_percent( length::parse(tmp) );
    } 
    catch(errors::invalid_attribute & ) {}

    BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
    return length_or_percent(length::parse(L"0cm"));
}

} 
}

