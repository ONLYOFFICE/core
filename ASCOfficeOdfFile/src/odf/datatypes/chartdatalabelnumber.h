#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

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
	std::wostream & operator << (std::wostream & _Wostream, const chart_data_label_number & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::chart_data_label_number);

}
