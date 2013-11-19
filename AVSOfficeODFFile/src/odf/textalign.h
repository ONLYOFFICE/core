#ifndef _CPDOCCORE_ODF_TEXTALIGN_H_
#define _CPDOCCORE_ODF_TEXTALIGN_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf { 

class text_align
{
public:
    enum type
    {
        Start, // start
        End, // end
        Left, // left
        Right, // right
        Center, // center
        Justify // justify
    };

    text_align() {}

    text_align(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_align parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_align & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::text_align);

}

#endif
