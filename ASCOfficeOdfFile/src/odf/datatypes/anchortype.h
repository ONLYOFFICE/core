#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class anchor_type
{
public:
    enum type
    {
        Page,
        Frame,
        Paragraph,
        Char,
        AsChar
    };

    anchor_type() {}

    anchor_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static anchor_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const anchor_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::anchor_type);

}

