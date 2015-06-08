#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"

namespace cpdoccore {
namespace odf_types { 

	// это  не только для text_align но и для horizontal_align подходит. .. нет надобности плодить лишнее 
class text_align
{
public:
    enum type
    {
        Start, // start
        End, // end
        Left, // left
        Right, // right
        Center, // center
        Justify // justify
    };

    text_align() {}

    text_align(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_align parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_align & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_align);

}
