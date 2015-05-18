
#include "targetframename.h"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>


namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const target_frame_name & _Val)
{
    switch (_Val.get_type())
    {
    default:
    case target_frame_name::Self:
        _Wostream << "_self";
        break;
    case target_frame_name::Blank:
        _Wostream << "_blank";
        break;
    case target_frame_name::Parent:
        _Wostream << "_parent";
        break;
    case target_frame_name::Top:
        _Wostream << "_top";
        break;
    case target_frame_name::String:
        _Wostream << _Val.get_name();
        break;
    }
    return _Wostream;
}

target_frame_name target_frame_name::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"_self")
        return target_frame_name(Self);
    else if (tmp == L"_blank")
        return target_frame_name(Blank);
    else if (tmp == L"_parent")
        return target_frame_name(Parent);
    else if (tmp == L"_top")
        return target_frame_name(Top);
    else 
        return target_frame_name(Str);                
}

}
}

