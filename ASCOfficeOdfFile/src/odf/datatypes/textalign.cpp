
#include "textalign.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const text_align & _Val)
{
    switch(_Val.get_type())
    {
    case text_align::Start:
        _Wostream << L"start";
        break;
    case text_align::End:
        _Wostream << L"end";
        break;
    case text_align::Left:
        _Wostream << L"left";
        break;
    case text_align::Right:
        _Wostream << L"right";
        break;
    case text_align::Center:
        _Wostream << L"center";
        break;
    case text_align::Justify:
        _Wostream << L"justify";
        break;
    default:
        break;
    }
    return _Wostream;    
}

text_align text_align::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"start")
        return text_align( Start );
    else if (tmp == L"end")
        return text_align( End );
    else if (tmp == L"left")
        return text_align( Left );
    else if (tmp == L"right")
        return text_align( Right );
    else if (tmp == L"center")
        return text_align( Center );
    else if (tmp == L"justify")
        return text_align( Justify );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return text_align( Start );
    }
}

} }
