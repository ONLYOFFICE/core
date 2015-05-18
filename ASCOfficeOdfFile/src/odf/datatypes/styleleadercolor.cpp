
#include "styleleadercolor.h"

#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const style_leader_color & _Val)
{
    switch(_Val.get_type())
    {
    case style_leader_color::Color:
        _Wostream << L"font-color";
        break;
    case style_leader_color::Enabled:
        _Wostream << _Val.get_color();
        break;
    default:
        break;
    }
    return _Wostream;    
}

style_leader_color style_leader_color::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"font-color")
        return style_leader_color( Color );
    else
        return style_leader_color( color::parse(Str) );
}

} }
