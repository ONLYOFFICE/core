#ifndef _CPDOCCORE_ODF_STYLEWRAP_H_
#define _CPDOCCORE_ODF_STYLEWRAP_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class style_wrap
{
public:
    enum type
    {
        None,
        Left,
        Right,
        Parallel,
        Dynamic,
        RunThrough,
        Biggest
    };

    style_wrap() {}

    style_wrap(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static style_wrap parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const style_wrap & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::style_wrap);

}

#endif
