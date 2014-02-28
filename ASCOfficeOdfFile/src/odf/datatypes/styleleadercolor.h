#ifndef _CPDOCCORE_ODF_STYLELEADERCOLOR_H_
#define _CPDOCCORE_ODF_STYLELEADERCOLOR_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"

#include "color.h"

namespace cpdoccore { namespace odf { 

class style_leader_color
{
public:
    enum type
    {
        Color,
        Enabled
    };

    style_leader_color() {}

    style_leader_color(type _Type) : type_(_Type)
    {}

    style_leader_color(const color & _Color) : color_(_Color), type_(Enabled)
    {}

    type get_type() const
    {
        return type_;
    };

    const color & get_color() const 
    {
        return color_;
    }
    
    static style_leader_color parse(const std::wstring & Str);

private:
    type type_;
    color color_;

};

std::wostream & operator << (std::wostream & _Wostream, const style_leader_color & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::style_leader_color);

}

#endif
