
#include "verticalalign.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const vertical_align & _Val)
{
    switch(_Val.get_type())
    {
    case vertical_align::Top:
        _Wostream << L"top";
        break;
    case vertical_align::Middle:
        _Wostream << L"middle";
        break;
    case vertical_align::Bottom:
        _Wostream << L"bottom";
        break;
    case vertical_align::Auto:
        _Wostream << L"auto";
        break;
    case vertical_align::Baseline:
        _Wostream << L"baseline";
        break;
    case vertical_align::Justify:
        _Wostream << L"justify";
        break;
    default:
        break;
    }
    return _Wostream;    
}

vertical_align vertical_align::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"top")
        return vertical_align( Top );
    else if (tmp == L"middle" || tmp == L"center")
        return vertical_align( Middle );
    else if (tmp == L"bottom")
        return vertical_align( Bottom );
    else if (tmp == L"auto" || tmp == L"automatic")
        return vertical_align( Auto );
    else if (tmp == L"baseline")
        return vertical_align( Baseline );
    else if (tmp == L"justify")
        return vertical_align( Justify );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return vertical_align( Baseline );
    }
}

} }
