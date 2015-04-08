#include "../precompiled_cpodf.h"
#include "styleverticalpos.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const vertical_pos & _Val)
{
    switch(_Val.get_type())
    {
    case vertical_pos::Top:
        _Wostream << L"top";
        break;
    case vertical_pos::Middle:
        _Wostream << L"middle";
        break;
    case vertical_pos::Bottom:
        _Wostream << L"bottom";
        break;
    case vertical_pos::FromTop:
        _Wostream << L"from-top";
        break;
    case vertical_pos::Below:
        _Wostream << L"below";
        break;
    default:
        break;
    }
    return _Wostream;    
}

vertical_pos vertical_pos::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"top")
        return vertical_pos( Top );
    else if (tmp == L"middle")
        return vertical_pos( Middle );
    else if (tmp == L"bottom")
        return vertical_pos( Bottom );
    else if (tmp == L"from-top")
        return vertical_pos( FromTop );
    else if (tmp == L"below")
        return vertical_pos( Below );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return vertical_pos( Top );
    }
}

} }
