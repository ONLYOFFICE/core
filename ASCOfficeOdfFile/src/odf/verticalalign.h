#pragma once


#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class vertical_align
{
public:
    enum type
    {
        Baseline,
        Top,
        Middle,
        Bottom,
        Auto,
        Justify
    };

    vertical_align() {}

    vertical_align(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    static vertical_align parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const vertical_align & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::vertical_align);

}

