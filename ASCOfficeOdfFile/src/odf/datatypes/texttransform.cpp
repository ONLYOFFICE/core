#include <boost/algorithm/string.hpp>

#include "texttransform.h"
#include "errors.h"

#include <iostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_transform & _Val)
{
    switch (_Val.get_type())
    {
    case text_transform::None:
        _Wostream << L"none";
        break;
    case text_transform::Lowercase:
        _Wostream << L"lowercase";
        break;
    case text_transform::Uppercase:
        _Wostream << L"uppercase";
        break;
    case text_transform::Capitalize:
        _Wostream << L"capitalize";
        break;
    default:
        break;
    }
    return _Wostream;
}

text_transform text_transform::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return text_transform(None);
    else if (tmp == L"lowercase")
        return text_transform(Lowercase);
    else if (tmp == L"uppercase")
        return text_transform(Uppercase);
    else if (tmp == L"capitalize")
        return text_transform(Capitalize);
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return None;        
    }
}

} }
