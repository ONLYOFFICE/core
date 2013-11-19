#ifndef _CPDOCCORE_ODF_CHARTSOLIDTYPE_H_
#define _CPDOCCORE_ODF_CHARTSOLIDTYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
//

namespace cpdoccore { namespace odf { 

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

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_solid_type);

}

#endif
