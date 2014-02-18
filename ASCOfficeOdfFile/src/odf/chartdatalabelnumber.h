#ifndef _CPDOCCORE_ODF_CHARTDATALABELNUMBER_H_
#define _CPDOCCORE_ODF_CHARTDATALABELNUMBER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class chart_data_label_number
{
public:
    enum type
    {
        none,
        value,
        percentage
    };

    chart_data_label_number() {}
    chart_data_label_number(type _Type) : type_(_Type) {}
    type get_type() const { return type_; };
    static chart_data_label_number parse(const std::wstring & Str);

private:
    type type_;
};

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_data_label_number);

}

#endif
