
#include "textalignsource.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const text_align_source & _Val)
{
    switch(_Val.get_type())
    {
    case text_align_source::Fix:
        _Wostream << L"fix";
        break;
    case text_align_source::ValueType:
        _Wostream << L"value-type";
        break;
    default:
        break;
    }
    return _Wostream;    
}

text_align_source text_align_source::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"fix")
        return text_align_source( Fix );
    else if (tmp == L"value-type")
        return text_align_source( ValueType );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return text_align_source( Fix );
    }
}

} }
