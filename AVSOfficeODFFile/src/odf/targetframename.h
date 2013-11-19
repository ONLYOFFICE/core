#ifndef _CPDOCCORE_ODF_TARGETFRAMENAME_H_
#define _CPDOCCORE_ODF_TARGETFRAMENAME_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class target_frame_name
{
public:
    enum type
    {
        Self, //_self
        Blank, // _blank
        Parent, // _parent
        Top, // _top
        String,
    };
    
public:
    target_frame_name() 
    {}

    target_frame_name(const std::wstring & _Name, type _Type) : name_(_Name), type_(_Type)
    {}

    target_frame_name(type _Type) : type_(_Type)
    {}

    target_frame_name(const std::wstring & _Name) : type_(String), name_(_Name)
    {}

    const ::std::wstring & get_name() const
    {
        return name_;
    }

    type get_type() const
    {
        return type_;
    };

    static target_frame_name parse(const std::wstring & Str);
    
private:
    type type_;
    ::std::wstring name_;
};

std::wostream & operator << (std::wostream & _Wostream, const target_frame_name & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::target_frame_name);


}

#endif
