#ifndef _CPDOCCORE_ODF_PUNCTUATIONWRAP_H_
#define _CPDOCCORE_ODF_PUNCTUATIONWRAP_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class punctuation_wrap
{
public:
    enum type
    {
        Simple,
        Hanging
    };

    punctuation_wrap() {}

    punctuation_wrap(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static punctuation_wrap parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const punctuation_wrap & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::punctuation_wrap);

}

#endif
