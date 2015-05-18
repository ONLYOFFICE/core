
#include "backgroundcolor.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const background_color & _Val)
{
    switch(_Val.get_type())
    {
    case background_color::Transparent:
        _Wostream << L"transparent";
        break;
    case background_color::Enabled:
        _Wostream << _Val.get_color();
        break;
    default:
        break;
    }
    return _Wostream;    
}

background_color background_color::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"transparent")
        return background_color( Transparent );
    else
        return background_color( color::parse(Str) );
}

} }
