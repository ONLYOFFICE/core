#ifndef _CPDOCCORE_ODF_LINESTYLE_H_
#define _CPDOCCORE_ODF_LINESTYLE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <iosfwd>
#include <cpdoccore/xml/attributes.h>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class line_style
{
public:
    enum style 
    {
        None,
        Solid,
        Dotted,
        Dash,
        LongDash,
        DotDash,
        DotDotDash,
        Wave
    };

    line_style()
    {}

    line_style(style _Style) : style_(_Style)
    {}

    style get_type() const 
    {
        return style_;
    }

    static line_style parse(const std::wstring & Str);

private:
    style style_;

};

std::wostream & operator << (std::wostream & _Wostream, const line_style & _LineStyle);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::line_style);

}

#endif
