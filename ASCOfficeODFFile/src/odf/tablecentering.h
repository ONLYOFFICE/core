#ifndef _CPDOCCORE_ODF_TABLECENTERING_H_
#define _CPDOCCORE_ODF_TABLECENTERING_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class table_centering
{
public:
    enum type
    {
        Horizontal,
        Vertical,
        Both,
        None
    };

    table_centering() {}

    table_centering(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static table_centering parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const table_centering & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::table_centering);

}

#endif
