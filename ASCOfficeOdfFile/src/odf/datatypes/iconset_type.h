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
        Arrows3Gray,
        Flags3,
		Signs3,
		Symbols3,
		Symbols3_2,
		Traffic3Lights1,
		Traffic3Lights2,
		Arrows4,
		Arrows4Gray,
		Rating4,
		RedToBlack4,
		Traffic4Lights,
		Arrows5,
		Arrows5Gray,
		Quarters5,
		Rating5
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

