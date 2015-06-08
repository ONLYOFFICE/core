#ifndef _CPDOCCORE_ODF_TEXTEMPHASIZE_H_
#define _CPDOCCORE_ODF_TEXTEMPHASIZE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class text_emphasize
{
public:
    enum type
    {
        None,
        Accent,
        Dot,
        Circle,
        Disc,
        Above,
        Below
    };

    text_emphasize() {}

    text_emphasize(type _Type1, type _Type2) : type1_(_Type1), type2_(_Type2)
    {}

    type get_type() const
    {
        return type1_;
    };

    type get_type_2() const
    {
        return type2_;
    };
    
    static text_emphasize parse(const std::wstring & Str);

private:
    type type1_;
    type type2_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_emphasize & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_emphasize);

}

#endif
