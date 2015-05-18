
#include <ostream>
#include <boost/algorithm/string.hpp>

#include "styleposition.h"

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const style_position & _Val)
{
    switch(_Val.get_horisontal())
    {
    case style_position::Left:
        _Wostream << L"left ";
        break;
    case style_position::Center:
        _Wostream << L"center ";
        break;
    case style_position::Right:
        _Wostream << L"right ";
        break;
    default:
        break;
    }

    switch(_Val.get_vertical())
    {
    case style_position::Top:
        _Wostream << L"top";
        break;
    case style_position::Center:
        _Wostream << L"center";
        break;
    case style_position::Bottom:
        _Wostream << L"bottom";
        break;
    default:
        break;
    }

    return _Wostream;    
}

style_position style_position::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);
    boost::algorithm::trim(tmp);
    std::vector< std::wstring > splitted;
    boost::algorithm::split(splitted, tmp, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

    type pos[2] = {Center, Center};
    int vertIndex = -1, horIndex = -1;

    for (size_t i = 0; i < (std::min)((size_t)2, splitted.size()); ++i)
    {
        if (splitted[i] == L"top" && -1 == vertIndex)
        {
            pos[1] = Top;
            vertIndex = (int)i;
        }
        else if (splitted[i] == L"bottom" && -1 == vertIndex)
        {
            pos[1] = Bottom;
            vertIndex = (int)i;
        }
        else if (splitted[i] == L"left" && -1 == horIndex)
        {
            pos[0] = Left;
            horIndex = (int)i;
        }
        else if (splitted[i] == L"right" && -1 == horIndex)
        {
            pos[0] = Right;
            horIndex = (int)i;
        }
        else if (splitted[i] == L"center")
        {}
        else
        {
            BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
        }
    }

    return style_position(pos[1], pos[0]);
}

} }
