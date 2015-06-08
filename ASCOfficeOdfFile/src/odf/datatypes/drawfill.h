#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { 
namespace odf_types { 

class draw_fill
{
public:
    enum type
    {
		none,
		solid,
		bitmap,
		gradient,
		hatch
    };

    draw_fill() {}

    draw_fill(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    static draw_fill parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const draw_fill & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::draw_fill);

}
