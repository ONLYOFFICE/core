#ifndef _CPDOCCORE_ODF_TEXTTRANSFORM_H_
#define _CPDOCCORE_ODF_TEXTTRANSFORM_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class text_transform
{
public:

    enum type
    {
        None,
        Lowercase,
        Uppercase,
        Capitalize
    };
    
    text_transform()
    {}

    text_transform(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };

    static text_transform parse(const std::wstring & Str);

private:
    type type_;
};

std::wostream & operator << (std::wostream & _Wostream, const text_transform & _Val);

}

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_transform);

}

#endif
