#ifndef _CPDOCCORE_ODF_WRITINGMODE_H_
#define _CPDOCCORE_ODF_WRITINGMODE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class writing_mode
{
public:
    enum type
    {
        LrTb,
        RlTb,
        TbRl,
        TbLr,
        Lr,
        Rl,
        Tb,
        Page,
    };

    writing_mode() {}

    writing_mode(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static writing_mode parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const writing_mode & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::writing_mode);

}

#endif
