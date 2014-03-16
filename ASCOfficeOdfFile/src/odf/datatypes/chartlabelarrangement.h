#ifndef _CPDOCCORE_ODF_CHARTLABELARRANGEMENT_H_
#define _CPDOCCORE_ODF_CHARTLABELARRANGEMENT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class chart_label_arrangement
{
public:
    enum type
    {
        side_by_side,
        stagger_even,
        stagger_odd
    };

    chart_label_arrangement() {}

    chart_label_arrangement(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static chart_label_arrangement parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const chart_label_arrangement & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_label_arrangement);

}

#endif
