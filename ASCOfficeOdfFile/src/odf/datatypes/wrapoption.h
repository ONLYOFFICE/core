#ifndef _CPDOCCORE_ODF_WRAPOPTION_H_
#define _CPDOCCORE_ODF_WRAPOPTION_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class wrap_option
{
public:
    enum type
    {
        NoWrap,
        Wrap
    };

    wrap_option() {}

    wrap_option(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static wrap_option parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const wrap_option & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::wrap_option);

}

#endif
