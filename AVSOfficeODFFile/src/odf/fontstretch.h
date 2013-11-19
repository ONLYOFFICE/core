#ifndef _CPDOCCORE_ODF_FONTSTRETCH_H_
#define _CPDOCCORE_ODF_FONTSTRETCH_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class font_stretch
{
public:
    enum type
    {
        Normal,
        UltraCondensed,
        ExtraCondensed,
        Condensed,
        SemiCondensed,
        SemiExpanded,
        Expanded,
        ExtraExpanded,
        UltraExpanded
    };

    font_stretch() {}

    font_stretch(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static font_stretch parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_stretch & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::font_stretch);

}

#endif
