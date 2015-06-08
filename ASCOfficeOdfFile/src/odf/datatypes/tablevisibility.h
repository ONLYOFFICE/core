#ifndef _CPDOCCORE_ODF_TABLEVISIBILITY_H_
#define _CPDOCCORE_ODF_TABLEVISIBILITY_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class table_visibility
{
public:
    enum type
    {
        Visible,
        Collapse,
        Filter
    };

    table_visibility() {}

    table_visibility(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static table_visibility parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const table_visibility & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::table_visibility);

}

#endif
