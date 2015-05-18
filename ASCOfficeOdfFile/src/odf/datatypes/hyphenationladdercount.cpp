
#include "hyphenationladdercount.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const integer_or_nolimit & _Val)
{
    switch(_Val.get_type())
    {
    case integer_or_nolimit::NoLimit:
        _Wostream << L"no-limit";
        break;
    case integer_or_nolimit::Integer:
        _Wostream << _Val.get_value();
        break;
    default:
        break;
    }
    return _Wostream;    
}

integer_or_nolimit integer_or_nolimit::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"no-limit")
        return integer_or_nolimit( NoLimit );
    else 
    {
        try 
        {
            return integer_or_nolimit( boost::lexical_cast<unsigned int>(Str) );
        } 
        catch(...)
        {}
    }

    BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
    return integer_or_nolimit( NoLimit );
}

} }
