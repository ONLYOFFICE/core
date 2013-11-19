#ifndef _CPDOCCORE_ODF_LETTERSPACING_H_
#define _CPDOCCORE_ODF_LETTERSPACING_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "length.h"


namespace cpdoccore { namespace odf { 

class letter_spacing
{
public:
    enum type
    {
        Normal,
        Length
    };

    letter_spacing() : type_(Normal) {}
    letter_spacing(type _Type) : type_(_Type) {}

    letter_spacing(const length & _Length) : type_(Length), length_(_Length)
    {}

    type get_type() const
    {
        return type_;
    };

    const length & get_length() const
    {
        return length_;
    }

    static letter_spacing parse(const std::wstring & Str);

private:
    type type_;
    length length_;

};

std::wostream & operator << (std::wostream & _Wostream, const letter_spacing & _Val);

}

APPLY_PARSE_XML_ATTRIBUTES(odf::letter_spacing);

}

#endif
