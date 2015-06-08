#ifndef _CPDOCCORE_ODF_TEXTCOMBINE_H_
#define _CPDOCCORE_ODF_TEXTCOMBINE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class text_combine
{
public:
    enum type
    {
        None,
        Letters,
        Lines
    };

    text_combine() {}

    text_combine(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_combine parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_combine & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_combine);

}

#endif
