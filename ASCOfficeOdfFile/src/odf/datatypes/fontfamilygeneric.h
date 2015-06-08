#ifndef _CPDOCCORE_ODF_FONTFAMILYGENERIC_H_
#define _CPDOCCORE_ODF_FONTFAMILYGENERIC_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class font_family_generic
{
public:
    enum type
    {
        Roman,
        Swiss,
        Modern,
        Decorative,
        Script,
        System
    };

    font_family_generic() {}

    font_family_generic(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static font_family_generic parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_family_generic & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::font_family_generic);

}

#endif // #ifndef _CPDOCCORE_ODF_FONTFAMILYGENERIC_H_
