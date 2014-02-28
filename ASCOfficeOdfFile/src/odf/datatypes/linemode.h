#ifndef _CPDOCCORE_ODF_LINEMODE_H_
#define _CPDOCCORE_ODF_LINEMODE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <iosfwd>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class line_mode
{
public:
    enum type
    {
        Continuous,
        SkipWhiteSpace
    };

    line_mode()
    {}

    line_mode(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };

    static line_mode parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const line_mode & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::line_mode);

}

#endif
