#pragma once

#include <string>
#include "odfattributes.h"

namespace cpdoccore { namespace odf_types { 

class presentation_class
{
public:
    enum type
    {
        page,
        title,
        outline,
        subtitle,
		text,
		graphic,
		object,
		chart,
		table,
		orgchart,
		notes,
		handout,
		header,
		footer,
		date_time,
		page_number

    };

    presentation_class() {}

    presentation_class(type _Type) : type_(_Type)
    {}

    type get_type() const
    {
        return type_;
    };
    
    static presentation_class parse(const std::wstring & Str);
	std::wstring get_type_ms();

private:
    type type_;

};
	std::wostream & operator << (std::wostream & _Wostream, const presentation_class & _Val);


}
	APPLY_PARSE_XML_ATTRIBUTES(odf_types::presentation_class);
}
