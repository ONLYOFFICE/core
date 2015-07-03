#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class color
{
public:
    color() {};
    
    color(const std::wstring & _Color) : color_(_Color) {};
    
    const std::wstring & get_color() const 
    { 
        return color_; 
    };

    const std::wstring get_hex_value() const;

    static color parse(const std::wstring & Str);

private:
    std::wstring color_;
};

std::wostream & operator << (std::wostream & _Wostream, const color & _Color);
bool            operator== (const color & c1, const color & c2);
} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::color);

}
