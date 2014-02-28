#ifndef _CPDOCCORE_ODF_LINETYPE_H_
#define _CPDOCCORE_ODF_LINETYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <iosfwd>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class line_type
{
public:
    enum type
    {
        None,
        Single,
        Double
    };

    line_type()
    {}

    line_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };

    static line_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const line_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::line_type);

}

#endif
