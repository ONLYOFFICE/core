#ifndef _CPDOCCORE_ODF_FOBREAK_H_
#define _CPDOCCORE_ODF_FOBREAK_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class fo_break
{
public:
    enum type
    {
        Auto,
        Column,
        Page
    };

    fo_break() {}

    fo_break(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static fo_break parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const fo_break & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::fo_break);

}

#endif
