#ifndef _CPDOCCORE_ODF_HORIZONTALPOS_H_
#define _CPDOCCORE_ODF_HORIZONTALPOS_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class horizontal_pos
{
public:
    enum type
    {
        Left,
        Center,
        Right,
        FromLeft,
        Inside,
        Outside,
        FromInside
    };

    horizontal_pos() {}

    horizontal_pos(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static horizontal_pos parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const horizontal_pos & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::horizontal_pos);

}

#endif
