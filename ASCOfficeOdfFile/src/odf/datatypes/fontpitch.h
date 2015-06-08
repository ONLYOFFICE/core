#ifndef _CPDOCCORE_ODF_FONTPITCH_H_
#define _CPDOCCORE_ODF_FONTPITCH_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class font_pitch
{
public:
    enum type
    {
        Fixed,
        Variable,
    };

    font_pitch() {}

    font_pitch(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static font_pitch parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const font_pitch & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::font_pitch);

}

#endif // #ifndef _CPDOCCORE_ODF_FONTPITCH_H_
