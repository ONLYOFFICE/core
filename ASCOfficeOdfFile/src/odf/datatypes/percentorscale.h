#ifndef _CPDOCCORE_ODF_PERCENTORSCALE_H_
#define _CPDOCCORE_ODF_PERCENTORSCALE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "percent.h"
#include "length.h"


namespace cpdoccore { namespace odf_types { 

class percent_or_scale
{
public:
    enum type
    {
        Percent,
        Scale,
        ScaleMin
    };

    percent_or_scale() {}

    percent_or_scale(percent _Percent) : type_(Percent), Percent_(_Percent)
    {}

    percent_or_scale(type _Type) : type_(_Type)
    {}

    type get_type() const 
    {
        return type_;
    }

    const percent & get_percent() const 
    {
        return Percent_;        
    }

    static percent_or_scale parse(const std::wstring & );

private:
    type type_;
    percent Percent_;
};

std::wostream & operator << (std::wostream & _Wostream, const percent_or_scale & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::percent_or_scale);

}

#endif
