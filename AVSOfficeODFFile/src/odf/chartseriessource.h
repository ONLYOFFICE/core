#ifndef _CPDOCCORE_ODF_CHARTSERIESSOURCE_H_
#define _CPDOCCORE_ODF_CHARTSERIESSOURCE_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class chart_series_source
{
public:
    enum type
    {
        columns,
        rows
    };

    chart_series_source() {}

    chart_series_source(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_series_source parse(const std::wstring & Str);

private:
    type type_;

};

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_series_source);

}

#endif
