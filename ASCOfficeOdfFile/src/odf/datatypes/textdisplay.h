#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class text_display
{
public:
    enum type
    {
        True,
        None,
        Condition
    };

    text_display() {}

    text_display(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static text_display parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const text_display & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_display);

}
