#ifndef _CPDOCCORE_ODF_DROPCAPLENGTH_H_
#define _CPDOCCORE_ODF_DROPCAPLENGTH_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "length.h"
#include "percent.h"


namespace cpdoccore { namespace odf_types { 

class drop_cap_length
{
public:
    enum type
    {
        Word,
        Integer
    };

    drop_cap_length() : type_(Integer), value_(1)  {}

    drop_cap_length(type _Type) : type_(_Type), value_(1)  {}

    drop_cap_length(unsigned int _Length) : type_(Integer), value_(_Length) {}

    type get_type() const
    {
        return type_;
    };

    unsigned int get_value() const
    {
        return value_;
    }
    
    static drop_cap_length parse(const std::wstring & Str);

private:
    type type_;
    unsigned int value_;

};

std::wostream & operator << (std::wostream & _Wostream, const drop_cap_length & _Val);

}

APPLY_PARSE_XML_ATTRIBUTES(odf_types::drop_cap_length);

}

#endif
