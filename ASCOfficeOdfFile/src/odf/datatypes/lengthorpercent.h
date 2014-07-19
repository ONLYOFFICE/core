#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "percent.h"
#include "length.h"


namespace cpdoccore { namespace odf { 

class length_or_percent
{
public:
    enum type
    {
        Length,
        Percent
    };

    length_or_percent() {}

    length_or_percent(percent _Percent) : type_(Percent), Percent_(_Percent)
    {}

    length_or_percent(length _Length) : type_(Length), Length_(_Length)
    {}

    type get_type() const 
    {
        return type_;
    }

    const percent & get_percent() const 
    {
        return Percent_;        
    }

    const length & get_length() const 
    {
        return Length_;
    }

    percent & get_percent() 
    {
        return Percent_;        
    }

    length & get_length() 
    {
        return Length_;
    }
    static length_or_percent parse(const std::wstring & );

private:
    type type_;
    percent Percent_;
    length Length_;
};

std::wostream & operator << (std::wostream & _Wostream, const length_or_percent & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::length_or_percent);

}
