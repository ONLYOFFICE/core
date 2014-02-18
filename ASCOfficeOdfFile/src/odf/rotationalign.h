#ifndef _CPDOCCORE_ODF_ROTATIONALIGN_H_
#define _CPDOCCORE_ODF_ROTATIONALIGN_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class rotation_align
{
public:
    enum type
    {
        None,
        Bottom,
        Top,
        Center
    };

    rotation_align() {}

    rotation_align(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static rotation_align parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const rotation_align & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::rotation_align);

}

#endif
