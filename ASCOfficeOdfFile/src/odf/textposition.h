#ifndef _CPDOCCORE_ODF_TEXTPOSITION_H_
#define _CPDOCCORE_ODF_TEXTPOSITION_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <iosfwd>
#include "percent.h"


namespace cpdoccore { namespace odf { 

class text_position
{
public:
    enum type 
    {
        Percent,
        Sub,
        Super
    };

public:
    text_position()
    {}

    text_position(type _Type) : type_(_Type), 
        has_font_size_(false)
    {}

    text_position(double _Percent) : type_(Percent), 
        position_(_Percent), 
        has_font_size_(false)
    {}

    text_position(type _Type, double _FontSize) : type_(_Type), 
        has_font_size_(true), 
        font_size_(_FontSize)
    {}

    text_position(double _Percent, double _FontSize) : type_(Percent), 
        position_(_Percent), 
        has_font_size_(true), 
        font_size_(_FontSize)
    {}

    type get_type() const
    {
        return type_;
    }

    const percent & get_position() const
    {
        return position_;
    }

    bool has_font_size() const
    {
        return has_font_size_; 
    }

    const percent & font_size() const
    {
        return font_size_;
    }

    static text_position parse(const std::wstring & Str);

private:
    type type_;
    percent position_;
    
    bool has_font_size_;
    percent font_size_;
    
};

std::wostream & operator << (std::wostream & _Wostream, const text_position & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::text_position);

}

#endif
