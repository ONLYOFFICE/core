#ifndef _CPDOCCORE_ODF_BORDERMODEL_H_
#define _CPDOCCORE_ODF_BORDERMODEL_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class border_model
{
public:
    enum type
    {
        Collapsing,
        Separating
    };

    border_model() {}

    border_model(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static border_model parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const border_model & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::border_model);

}

#endif
