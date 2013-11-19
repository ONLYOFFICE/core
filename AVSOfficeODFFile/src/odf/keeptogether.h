#ifndef _CPDOCCORE_ODF_KEEPTOGETHER_H_
#define _CPDOCCORE_ODF_KEEPTOGETHER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class keep_together
{
public:
    enum type
    {
        Auto,
        Always
    };

    keep_together() {}

    keep_together(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static keep_together parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const keep_together & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::keep_together);

}

#endif
