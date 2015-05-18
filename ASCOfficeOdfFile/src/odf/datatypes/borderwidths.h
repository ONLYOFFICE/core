#pragma once


#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "length.h"


namespace cpdoccore { namespace odf { 

class border_widths
{
public:
    border_widths() {}

    border_widths(length _Len1, length _Len2, length _Len3) : len1_(_Len1), len2_(_Len2), len3_(_Len3)
    {}
    
    static border_widths parse(const std::wstring & Str);

    const length & get_len1() const
    {
        return len1_;
    }
    
    const length & get_len2() const
    {
        return len2_;
    }

    const length & get_len3() const
    {
        return len3_;
    }

    double get_summ_unit(length::unit Unit) const
    {
        return len1_.get_value_unit(Unit) + len2_.get_value_unit(Unit) + len3_.get_value_unit(Unit);
    }

private:
    length len1_;
    length len2_;
    length len3_;

};

std::wostream & operator << (std::wostream & _Wostream, const border_widths & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::border_widths);

}
