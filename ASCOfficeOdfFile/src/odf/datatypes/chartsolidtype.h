#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"
//

namespace cpdoccore { namespace odf_types { 

class chart_solid_type
{
public:
    enum type
    {
        cuboid,
        cylinder,
        cone,
        pyramid
    };

    chart_solid_type() {}

    chart_solid_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_solid_type parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const chart_solid_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::chart_solid_type);

}
