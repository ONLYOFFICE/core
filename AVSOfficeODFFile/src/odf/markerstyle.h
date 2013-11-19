#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { 
namespace odf { 

class marker_style
{
public:
    marker_style() {}

	marker_style(std::wstring _style) : style_(_style)
    {}

    std::wstring get() const
    {
        return style_;
    };
    
    static std::wstring parse(const std::wstring & Str);

private:
    std::wstring style_;

};

} 


APPLY_PARSE_XML_ATTRIBUTES(odf::marker_style);

}