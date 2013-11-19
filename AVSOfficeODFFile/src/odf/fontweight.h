#ifndef _CPDOCCORE_ODF_FONTWEIGHT_H_
#define _CPDOCCORE_ODF_FONTWEIGHT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class font_weight
{
public:
    enum type
    {
        WNormal,
        WBold,
        W100,
        W200,
        W300,
        W400,
        W500,
        W600,
        W700,
        W800,
        W900
    };

    font_weight() {}

    font_weight(type _Type) : type_(_Type)
    {}

    type get_type() const 
    {
        return type_;
    }

    static font_weight parse(const std::wstring & Str);

private:
    type type_;
};

std::wostream & operator << (std::wostream & _Wostream, const font_weight & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::font_weight);

}

#endif
