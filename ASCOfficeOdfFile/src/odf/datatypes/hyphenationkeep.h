#ifndef _CPDOCCORE_ODF_HYPHENATIONKEEP_H_
#define _CPDOCCORE_ODF_HYPHENATIONKEEP_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class hyphenation_keep
{
public:
    enum type
    {
        Auto,
        Page
    };

    hyphenation_keep() {}

    hyphenation_keep(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static hyphenation_keep parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const hyphenation_keep & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::hyphenation_keep);

}

#endif
