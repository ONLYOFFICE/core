#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { 
namespace odf { 

class fill_image_ref_point
{
public:
    enum type
    {
		top_left,
		top,
		top_right,
		left,
		center,
		right,
		bottom_left,
		bottom,
		bottom_right
    };

    fill_image_ref_point() {}

    fill_image_ref_point(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    static fill_image_ref_point parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const fill_image_ref_point & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::fill_image_ref_point);

}
