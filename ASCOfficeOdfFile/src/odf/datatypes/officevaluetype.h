#pragma once

#include <string>
#include <iosfwd>
#include "odfattributes.h"


namespace cpdoccore { 
namespace odf { 

class office_value_type
{
public:
    enum type
    {
		Custom,
		Float,
        Currency,
		Percentage,
        Date,
        Time,
		Boolean,
		String
    };

    office_value_type()  {type_ = String;}

    office_value_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };

    static office_value_type parse(const std::wstring & Str);

private:
    type type_;

};

std::wostream & operator << (std::wostream & _Wostream, const office_value_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::office_value_type);

}