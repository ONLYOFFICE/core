#ifndef _CPDOCCORE_ODF_TEXTROTATIONSCALE_H_
#define _CPDOCCORE_ODF_TEXTROTATIONSCALE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class text_rotation_scale
{
public:
    enum type
    {
        Fixed,
        LineHeight
    };

    text_rotation_scale() {}

    text_rotation_scale(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_rotation_scale parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_rotation_scale & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_rotation_scale);

}

#endif
