#include "precompiled_cpodf.h"
#include "dropcaplength.h"

#include <boost_string.h>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const drop_cap_length & _Val)
{
    switch(_Val.get_type())
    {
    case drop_cap_length::Word:
        _Wostream << "word";
        break;
    case drop_cap_length::Integer:
        _Wostream << _Val.get_value();
        break;
    default:
        break;
    }
    return _Wostream;
}

drop_cap_length drop_cap_length::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (L"word" == tmp)
    {
        try 
        {
            return drop_cap_length( Word );
        }
        catch(errors::invalid_attribute &) {}
    }

    try 
    {
        return drop_cap_length( boost::lexical_cast<unsigned int>(tmp) );
    } 
    catch(...)
    {
    }

    BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
    return drop_cap_length( 1 );
}

} }

