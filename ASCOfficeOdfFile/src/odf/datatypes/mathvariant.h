#pragma once

#include <string>
#include <cpdoccore/CPOptional.h>
#include "odfattributes.h"

namespace cpdoccore { namespace odf_types { 

class math_variant
{
public:
    math_variant(){}

    math_variant(const std::wstring & Value);
    math_variant(const math_variant & Value);

    static math_variant parse(const std::wstring & Str);
   
	math_variant& operator=(const math_variant& d);

    bool is_none()     const    { return none_; }

	struct _style
	{
		_style() : italic(false), bold(false), type(0) {}
		bool italic;
		bool bold;
		int  type; 
	}style_;

private:
	bool none_;

};

std::wostream & operator << (std::wostream & _Wostream, const math_variant & _Val);
bool            operator == (math_variant & _b1, math_variant & _b2);

} 
    APPLY_PARSE_XML_ATTRIBUTES(odf_types::math_variant);

}
