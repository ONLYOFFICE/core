#pragma once

#include <string>
#include "length.h"
#include "color.h"
#include <cpdoccore/CPOptional.h>

namespace cpdoccore { namespace odf_types { 

class border_style
{
public:
    enum type
    {
        none,
        solid,
        double_,
        dotted,
        dashed,
        dot_dashed,
        long_dash,
        dot_dash,
        dot_dot_dash,
        single,
        groove,
        ridge,
        inset,
        outset,
        hidden

    };

    border_style(){none_ = true;}

    border_style(const std::wstring & Value);
    border_style(const border_style & Value);
    border_style(const color & color_,  const type & style_, const length & length_);

    static border_style parse(const std::wstring & Str);

public:
    bool initialized() const    { return initialized_; }
    bool is_none()     const    { return none_; }

    const length     & get_length()const { return length_; }
    const type       & get_style() const { return style_; }
    const color      & get_color() const { return color_; }

private:
    bool        none_;
    bool        initialized_;
    length      length_;
    type        style_;
    color       color_;
};

std::wostream & operator << (std::wostream & _Wostream, const border_style & _Val);

} 
    APPLY_PARSE_XML_ATTRIBUTES(odf_types::border_style);

}
