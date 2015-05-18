
#include "textcombine.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const text_combine & _Val)
{
    switch(_Val.get_type())
    {
    case text_combine::None:
        _Wostream << L"none";
        break;
    case text_combine::Letters:
        _Wostream << L"letters";
        break;
    case text_combine::Lines:
        _Wostream << L"lines";
        break;
    default:
        break;
    }
    return _Wostream;    
}

text_combine text_combine::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return text_combine( None );
    else if (tmp == L"letters")
        return text_combine( Letters );
    else if (tmp == L"lines")
        return text_combine( Lines );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return text_combine( None );
    }
}

} }
