#include "precompiled_cpodf.h"
#include "underlinecolor.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const underline_color & _Val)
{
    switch(_Val.get_type())
    {
    case underline_color::FontColor:
        _Wostream << L"font-color";
        break;
    case underline_color::Enabled:
        _Wostream << _Val.get_color();
        break;
    default:
        break;
    }
    return _Wostream;    
}

underline_color underline_color::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"font-color")
        return underline_color( FontColor );
    else
        return underline_color( color::parse(Str) );
}

} }
