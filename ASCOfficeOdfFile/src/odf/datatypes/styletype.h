#ifndef _CPDOCCORE_ODF_STYLETYPE_H_
#define _CPDOCCORE_ODF_STYLETYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class style_type
{
public:
    enum type
    {
        Left,
        Center,
        Right,
        Char
    };

    style_type() {}

    style_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static style_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const style_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::style_type);

}

#endif
