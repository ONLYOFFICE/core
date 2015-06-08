#pragma once

#include <iosfwd>
#include <string>
#include "odfattributes.h"


namespace cpdoccore { namespace odf_types { 

class Bool
{
public:
	Bool() {bool_ = false;};
    
    Bool(bool Val) : bool_(Val) { bool_str_ = bool_ ? L"true" : L"false"; };
    
    const bool & get() const 
    { 
        return bool_; 
    };
	
	void operator=( bool b ) {bool_ = b; bool_str_ = bool_ ? L"true" : L"false"; }
	
	const std::wstring &string() const 
	{
		return bool_str_ ;
	}
	static Bool parse(const std::wstring & Str);


private:
    bool bool_;
	std::wstring bool_str_;
};

std::wostream & operator << (std::wostream & _Wostream, const Bool & _val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::Bool);

}
