#ifndef _CPDOCCORE_ODF_TABLEALIGN_H_
#define _CPDOCCORE_ODF_TABLEALIGN_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class table_align
{
public:
    enum type
    {
        Left,
        Center,
        Right,
        Margins
    };

    table_align() {}

    table_align(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static table_align parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const table_align & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::table_align);

}

#endif
