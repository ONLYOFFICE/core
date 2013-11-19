#ifndef _CPDOCCORE_ODF_TABLEMODE_H_
#define _CPDOCCORE_ODF_TABLEMODE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class table_mode
{
public:
    enum type
    {
        CopyAll,
        CopyResultsOnly
    };

    table_mode() {}

    table_mode(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static table_mode parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const table_mode & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::table_mode);

}

#endif
