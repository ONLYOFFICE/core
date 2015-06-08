#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { 
namespace odf_types { 

class gradient_style
{
public:
    enum type
    {
		linear,
		axial,
		radial,
		ellipsoid,
		square,
		rectangular
    };

    gradient_style() {}

    gradient_style(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    static gradient_style parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const gradient_style & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::gradient_style);

}
