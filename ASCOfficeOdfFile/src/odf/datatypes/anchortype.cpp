
#include "anchortype.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const anchor_type & _Val)
{
    switch(_Val.get_type())
    {
    case anchor_type::Page:
        _Wostream << L"page";
        break;
    case anchor_type::Frame:
        _Wostream << L"frame";
        break;
    case anchor_type::Paragraph:
        _Wostream << L"paragraph";
        break;
    case anchor_type::Char:
        _Wostream << L"char";
        break;
    case anchor_type::AsChar:
        _Wostream << L"as-char";
        break;
    default:
        break;
    }
    return _Wostream;    
}

anchor_type anchor_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"page")
        return anchor_type( Page );
    else if (tmp == L"frame")
        return anchor_type( Frame );
    else if (tmp == L"paragraph")
        return anchor_type( Paragraph );
    else if (tmp == L"char")
        return anchor_type( Char );
    else if (tmp == L"as-char")
        return anchor_type( AsChar );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return anchor_type( Page );
    }
}

} }
