#ifndef _CPDOCCORE_ODF_LINEBREAK_H_
#define _CPDOCCORE_ODF_LINEBREAK_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class line_break
{
public:
    enum type
    {
        Normal,
        Strict
    };

    line_break() {}

    line_break(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static line_break parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const line_break & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::line_break);

}

#endif
