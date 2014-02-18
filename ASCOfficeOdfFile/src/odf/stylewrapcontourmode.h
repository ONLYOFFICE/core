#ifndef _CPDOCCORE_ODF_WRAP_CONTOUR_MODE_H_
#define _CPDOCCORE_ODF_WRAP_CONTOUR_MODE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class wrap_contour_mode
{
public:
    enum type
    {
        Full,
        Outside
    };

    wrap_contour_mode() {}

    wrap_contour_mode(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static wrap_contour_mode parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const wrap_contour_mode & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::wrap_contour_mode);

}

#endif
