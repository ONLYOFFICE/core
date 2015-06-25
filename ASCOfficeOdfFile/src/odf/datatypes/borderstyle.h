#pragma once

#include <string>
#include "length.h"
#include "color.h"

namespace cpdoccore { namespace odf_types { 

class border_style
{
public:
	border_style(){}
	border_style(const std::wstring & Value);
	border_style(const color & color_,  const std::wstring & style_, const length & length_);
    static border_style parse(const std::wstring & Str);

public:
    bool initialized() const { return initialized_; }
    const length & get_length() const { return length_; }
    const std::wstring & get_style() const { return style_; }
    const color & get_color() const { return color_; }
    bool is_none() const { return none_; }

	std::wstring set_border_style_;
private:
	bool none_;
    bool initialized_;
    length length_;
    std::wstring style_;
    color color_;
};

std::wostream & operator << (std::wostream & _Wostream, const border_style & _Val);


} 
APPLY_PARSE_XML_ATTRIBUTES(odf_types::border_style);

}
