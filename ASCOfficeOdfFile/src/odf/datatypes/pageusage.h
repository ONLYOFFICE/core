#ifndef _CPDOCCORE_ODF_PAGEUSAGE_H_
#define _CPDOCCORE_ODF_PAGEUSAGE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class page_usage
{
public:
    enum type
    {
        All,
        Left,
        Right,
        Mirrored
    };

    page_usage() {}

    page_usage(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static page_usage parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const page_usage & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::page_usage);

}

#endif
