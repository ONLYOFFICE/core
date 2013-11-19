#ifndef _CPDOCCORE_ODF_FONTSIZE_H_
#define _CPDOCCORE_ODF_FONTSIZE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "length.h"
#include "percent.h"


namespace cpdoccore { namespace odf { 

class font_size
{
public:
    enum type
    {
        Length,
        Percent
    };

    font_size() {}

    font_size(const length & _Length) : type_(Length), length_(_Length)
    {}

    font_size(const percent & _Percent) : type_(Percent), percent_(_Percent)
    {}

    type get_type() const
    {
        return type_;
    };

    const length & get_length() const
    {
        return length_;
    }

    const percent & get_percent() const 
    {
        return percent_;
    }
    
    static font_size parse(const std::wstring & Str);

private:
    type type_;
    length length_;
    percent percent_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_size & _Val);

}

APPLY_PARSE_XML_ATTRIBUTES(odf::font_size);

}

#endif
