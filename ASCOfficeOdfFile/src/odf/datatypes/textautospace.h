#ifndef _CPDOCCORE_ODF_TEXTAUTOSPACE_H_
#define _CPDOCCORE_ODF_TEXTAUTOSPACE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class text_autospace
{
public:
    enum type
    {
        None,
        IdeographAlpha
    };

    text_autospace() {}

    text_autospace(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_autospace parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_autospace & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_autospace);

}

#endif
