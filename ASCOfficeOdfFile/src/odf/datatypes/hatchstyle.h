#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { 
namespace odf_types { 

class hatch_style
{
public:
    enum type
    {
		single,
		doublee,
		triple
    };

    hatch_style() {}

    hatch_style(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    static hatch_style parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const hatch_style & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::hatch_style);

}
