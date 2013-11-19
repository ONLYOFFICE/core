#ifndef _CPDOCCORE_ODF_FONTVARIANT_H_
#define _CPDOCCORE_ODF_FONTVARIANT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class font_variant
{
public:
    enum type
    {
        Normal,
        SmallCaps
    };

    font_variant() {}

    font_variant(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static font_variant parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_variant & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::font_variant);

}

#endif
