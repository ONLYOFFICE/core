#ifndef _CPDOCCORE_ODF_FONTRELIEF_H_
#define _CPDOCCORE_ODF_FONTRELIEF_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class font_relief
{
public:
    enum type
    {
        None,
        Embossed,
        Engraved
    };

    font_relief() {}

    font_relief(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static font_relief parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_relief & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::font_relief);

}

#endif
