#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class calcext_type
{
public:
    enum type
    {
        Percent,
        Number,
        Maximum,
        Minimum,
        AutoMaximum,
        AutoMinimum
    };

    calcext_type() {}

    calcext_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static calcext_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const calcext_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::calcext_type);

}
