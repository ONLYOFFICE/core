#ifndef _CPDOCCORE_ODF_CHARTINTERPOLATION_H_
#define _CPDOCCORE_ODF_CHARTINTERPOLATION_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class chart_interpolation
{
public:
    enum type
    {
        none,
        cubicSpline,
        bSpline
    };

    chart_interpolation() {}

    chart_interpolation(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_interpolation parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const chart_interpolation & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_interpolation);

}

#endif
