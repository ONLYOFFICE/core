#ifndef _CPDOCCORE_ODF_UNDERLINECOLOR_H_
#define _CPDOCCORE_ODF_UNDERLINECOLOR_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"

#include "color.h"

namespace cpdoccore { namespace odf_types { 

class underline_color
{
public:
    enum type
    {
        FontColor,
        Enabled
    };

    underline_color() {}

    underline_color(type _Type) : type_(_Type)
    {}

    underline_color(const color & _Color) : color_(_Color), type_(Enabled)
    {}

    type get_type() const
    {
        return type_;
    };

    const color & get_color() const 
    {
        return color_;
    }
    
    static underline_color parse(const std::wstring & Str);

private:
    type type_;
    color color_;

};

std::wostream & operator << (std::wostream & _Wostream, const underline_color & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::underline_color);

}

#endif
