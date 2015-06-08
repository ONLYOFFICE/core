
#include "fontstretch.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const font_stretch & _Val)
{
    switch(_Val.get_type())
    {
    case font_stretch::Normal:
        _Wostream << L"normal";
        break;
    case font_stretch::UltraCondensed:
        _Wostream << L"ultra-condensed";
        break;
    case font_stretch::ExtraCondensed:
        _Wostream << L"extra-condensed";
        break;
    case font_stretch::Condensed:
        _Wostream << L"condensed";
        break;
    case font_stretch::SemiCondensed:
        _Wostream << L"semi-condensed";
        break;
    case font_stretch::SemiExpanded:
        _Wostream << L"semi-expanded";
        break;
    case font_stretch::Expanded:
        _Wostream << L"expanded";
        break;
    case font_stretch::ExtraExpanded:
        _Wostream << L"extra-expanded";
        break;
    case font_stretch::UltraExpanded:
        _Wostream << L"ultra-expanded";
        break;
    default:
        break;
    }
    return _Wostream;    
}

font_stretch font_stretch::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"normal")
        return font_stretch( Normal );
    else if (tmp == L"ultra-condensed")
        return font_stretch( UltraCondensed );
    else if (tmp == L"extra-condensed")
        return font_stretch( ExtraCondensed );
    else if (tmp == L"condensed")
        return font_stretch( Condensed );
    else if (tmp == L"semi-condensed")
        return font_stretch( SemiCondensed );
    else if (tmp == L"semi-expanded")
        return font_stretch( SemiExpanded );
    else if (tmp == L"expanded")
        return font_stretch( Expanded );
    else if (tmp == L"extra-expanded")
        return font_stretch( ExtraExpanded );
    else if (tmp == L"ultra-expanded")
        return font_stretch( UltraExpanded );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return font_stretch( Normal );
    }
}

} }
