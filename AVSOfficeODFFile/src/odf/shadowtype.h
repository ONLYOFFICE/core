#ifndef _CPDOCCORE_ODF_SHADOWTYPE_H_
#define _CPDOCCORE_ODF_SHADOWTYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class shadow_type
{
public:
    enum type
    {
        None,
        Enable
    };

    shadow_type() {}

    shadow_type(type _Type) : type_(_Type)
    {}

    shadow_type(const std::wstring & _Name) : type_(Enable), name_(_Name)
    {}

    type get_type() const
    {
        return type_;
    };

    const std::wstring & get_name() const
    {
        return name_;
    }
    
    static shadow_type parse(const std::wstring & Str);

private:
    type type_;
    std::wstring name_;

};

std::wostream & operator << (std::wostream & _Wostream, const shadow_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::shadow_type);

}

#endif
