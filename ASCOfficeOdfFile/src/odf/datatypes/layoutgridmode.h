#ifndef _CPDOCCORE_ODF_LAYOUTGRIDMODE_H_
#define _CPDOCCORE_ODF_LAYOUTGRIDMODE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class layout_grid_mode
{
public:
    enum type
    {
        None,
        Line,
        Both
    };

    layout_grid_mode() {}

    layout_grid_mode(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static layout_grid_mode parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const layout_grid_mode & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::layout_grid_mode);

}

#endif
