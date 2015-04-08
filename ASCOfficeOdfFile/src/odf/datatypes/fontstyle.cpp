#include "../precompiled_cpodf.h"
#include "fontstyle.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const font_style & _Val)
{
    switch(_Val.get_type())
    {
    case font_style::Normal:
        _Wostream << L"normal";
        break;
    case font_style::Italic:
        _Wostream << L"italic";
        break;
    case font_style::Oblique:
        _Wostream << L"oblique";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_style font_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"normal")
        return font_style( Normal );
    else if (tmp == L"italic")
        return font_style( Italic );
    else if (tmp == L"oblique")
        return font_style( Oblique );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return font_style( Normal );
    }
}

} }
