#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class iconset_type
{
public:
    enum type
    {
        Arrows3,
        Arrows3Grey,
        Flags3,
		Traffic3Lights1,
		Traffic3Lights2
   };

    iconset_type() {}

    iconset_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static iconset_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const iconset_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::iconset_type);

}

