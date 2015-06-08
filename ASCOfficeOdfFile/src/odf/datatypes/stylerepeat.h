#ifndef _CPDOCCORE_ODF_STYLEREPEAT_H_
#define _CPDOCCORE_ODF_STYLEREPEAT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class style_repeat
{
public:
    enum type
    {
        NoRepeat,
        Repeat,
        Stretch
    };

    style_repeat() {}

    style_repeat(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static style_repeat parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const style_repeat & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::style_repeat);

}

#endif
