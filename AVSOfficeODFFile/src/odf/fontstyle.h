#ifndef _CPDOCCORE_ODF_FONTSTYLE_H_
#define _CPDOCCORE_ODF_FONTSTYLE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class font_style
{
public:
    enum type
    {
        Normal,
        Italic,
        Oblique
    };

    font_style() {}

    font_style(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static font_style parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_style & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::font_style);

}

#endif
