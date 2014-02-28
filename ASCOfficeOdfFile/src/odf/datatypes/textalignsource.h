#ifndef _CPDOCCORE_ODF_TEXTALIGNSOURCE_H_
#define _CPDOCCORE_ODF_TEXTALIGNSOURCE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class text_align_source
{
public:
    enum type
    {
        Fix,
        ValueType
    };

    text_align_source() {}

    text_align_source(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_align_source parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_align_source & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::text_align_source);

}

#endif
