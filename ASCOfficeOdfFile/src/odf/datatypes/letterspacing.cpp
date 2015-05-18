#include <ostream>
#include <boost/algorithm/string.hpp>

#include "letterspacing.h"

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const letter_spacing & _Val)
{
    switch(_Val.get_type())
    {
    case letter_spacing::Length:
        _Wostream << _Val.get_length();
        break;
    case letter_spacing::Normal:
        _Wostream << "normal";
        break;
    default:
        break;
    }
    return _Wostream;
}

letter_spacing letter_spacing::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (Str == L"normal")
        return letter_spacing(Normal);

    try 
    {
        return letter_spacing( length::parse(tmp) );
    } 
    catch(errors::invalid_attribute & ) {}

    BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
    return letter_spacing( Normal );
}

} }
