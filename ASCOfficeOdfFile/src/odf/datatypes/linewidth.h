#ifndef _CPDOCCORE_ODF_LINEWIDTH_H_
#define _CPDOCCORE_ODF_LINEWIDTH_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "lengthorpercent.h"



namespace cpdoccore { namespace odf_types { 

class line_width
{
public:
    enum type
    {
        Auto,
        Normal,
        Bold,
        Thin,
        Dash,
        Medium,
        Thick,

        PositiveInteger,
        Percent,
        PositiveLength
    };

    line_width() {}

    line_width(type _Type) : type_(_Type)
    {}

    line_width(unsigned int _Val) : type_(PositiveInteger), PositiveInteger_(_Val)
    {}
    line_width(length_or_percent _Val) ;
    line_width(percent _Percent) : type_(Percent), Percent_(_Percent)
    {}

    line_width(length _Length) : type_(PositiveLength), PositiveLength_(_Length)
    {}

    type get_type() const 
    {
        return type_;
    }
    
    unsigned int get_positive_integer() const 
    {
        return PositiveInteger_;
    }

    const percent & get_percent() const 
    {
        return Percent_;        
    }

    const length & get_positive_length() const 
    {
        return PositiveLength_;
    }

    static line_width parse(const std::wstring & );

private:
    type type_;
    unsigned int PositiveInteger_;
    percent Percent_;
    length PositiveLength_;
};

std::wostream & operator << (std::wostream & _Wostream, const line_width & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::line_width);

}

#endif
