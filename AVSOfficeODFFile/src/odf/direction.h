#ifndef _CPDOCCORE_ODF_STYLEDIRECTION_H_
#define _CPDOCCORE_ODF_STYLEDIRECTION_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class direction
{
public:
    enum type
    {
        Ltr,
        Ttb
    };

    direction() {}

    direction(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static direction parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const direction & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::direction);

}

#endif
