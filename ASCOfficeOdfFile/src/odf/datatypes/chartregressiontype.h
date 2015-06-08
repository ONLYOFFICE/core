#ifndef _CPDOCCORE_ODF_CHARTREGRESSION_TYPE_H_
#define _CPDOCCORE_ODF_CHARTREGRESSION_TYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class chart_regression_type
{
public:
    enum type
    {
        none, linear, logarithmic, exponential, power
    };

    chart_regression_type() {}

    chart_regression_type(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_regression_type parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const chart_regression_type & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::chart_regression_type);

}

#endif
