#ifndef _CPDOCCORE_ODF_BORDERSTYLE_H_
#define _CPDOCCORE_ODF_BORDERSTYLE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include "length.h"
#include "color.h"

namespace cpdoccore { namespace odf { 

class border_style
{
public:
    border_style(const std::wstring & Value);

public:
    bool initialized() const { return initialized_; }
    const length & get_length() const { return length_; }
    const std::wstring & get_style() const { return style_; }
    const color & get_color() const { return color_; }
    bool is_none() const { return none_; }

private:
    bool none_;
    bool initialized_;
    length length_;
    std::wstring style_;
    color color_;
};

} }

#endif
