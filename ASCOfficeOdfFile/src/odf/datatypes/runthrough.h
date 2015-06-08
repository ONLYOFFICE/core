#ifndef _CPDOCCORE_ODF_RUNTHROUGH_H_
#define _CPDOCCORE_ODF_RUNTHROUGH_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class run_through
{
public:
    enum type
    {
        Foreground,
        Background
    };

    run_through() {}

    run_through(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static run_through parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const run_through & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::run_through);

}

#endif
