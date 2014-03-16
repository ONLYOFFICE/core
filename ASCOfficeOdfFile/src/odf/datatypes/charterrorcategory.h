#ifndef _CPDOCCORE_ODF_CHARTERRORCATEGORY_H_
#define _CPDOCCORE_ODF_CHARTERRORCATEGORY_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf { 

class chart_error_category
{
public:
    enum type
    {
        none,
        variance,
        standard_deviation,
        percentage,
        error_margin,
        constant
    };

    chart_error_category() {}
    chart_error_category(type _Type) : type_(_Type)   {}
    type get_type() const   {  return type_; };
    static chart_error_category parse(const std::wstring & Str);

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const chart_error_category & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf::chart_error_category);

}

#endif
