#pragma once


//#include <iosfwd>
#include <string>
#include "odfattributes.h"

#include "color.h"

namespace cpdoccore { namespace odf_types { 

class background_color
{
public:
    enum type
    {
        Transparent,
        Enabled
    };

    background_color() {}

    background_color(type _Type) : type_(_Type)
    {}

    background_color(const color & _Color) : color_(_Color), type_(Enabled)
    {}

    type get_type() const
    {
        return type_;
    };

    const color & get_color() const 
    {
        return color_;
    }
    
    static background_color parse(const std::wstring & Str);

private:
    type type_;
    color color_;

};

std::wostream & operator << (std::wostream & _Wostream, const background_color & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::background_color);

}
