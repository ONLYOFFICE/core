#include <boost/algorithm/string.hpp>

#include "mathvariant.h"
#include <vector>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const math_variant & math_variant_)
{
    if (math_variant_.is_none())
    {
        return _Wostream;
    }

//.....
    return _Wostream;
}

bool operator == (math_variant & _b1, math_variant & _b2)
{
    if (_b1.is_none() && _b2.is_none()) return true;

    if (_b1.is_none() || _b2.is_none()) return false;

	if (    _b1.style_.bold     == _b2.style_.bold &&
			_b1.style_.italic	== _b2.style_.italic  &&
			_b1.style_.type     == _b2.style_.type)     return true;

    return false;
}


math_variant::math_variant(const math_variant & Value)
{
    style_  = Value.style_;

	none_ = Value.is_none();

}
math_variant& math_variant::operator =(const math_variant& Value)
{
    style_  = Value.style_;

    none_   = Value.is_none();

    return *this;
}
math_variant::math_variant(const std::wstring & Value) :  none_(true)
{
	std::wstring tmp =  boost::algorithm::trim_copy(Value);
    if (tmp.length() < 1) return;
   
	boost::algorithm::to_lower(tmp);
    
	std::vector< std::wstring > splitted;
	boost::algorithm::split(splitted, Value, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

	for (int i = 0 ; i < splitted.size(); i++)
	{
		if (splitted[i] == L"bold")
		{
			style_.bold = true;
		}
		else if (splitted[i] == L"italic")
		{
			style_.italic = true;
		}
		else if (splitted[i] == L"bold-italic")
		{
			style_.bold = true;
			style_.italic = true;
		}		
 		else if (splitted[i] == L"double-struck")
		{
			style_.type = 1;
		}
 		else if (splitted[i] == L"bold-fraktur")
		{
			style_.type = 2;
			style_.bold = true;
		}
  		else if (splitted[i] == L"fraktur")
		{
			style_.type = 2;
		}
		else if (splitted[i] == L"script")
		{
			style_.type = 3;
		}	
 		else if (splitted[i] == L"bold-script")
		{
			style_.type = 3;
			style_.bold = true;
		}	
 		else if (splitted[i] == L"sans-serif-italic")
		{
			style_.type = 4;
			style_.italic = true;
		}
 		else if (splitted[i] == L"bold-sans-serif")
		{
			style_.type = 4;
			style_.bold = true;
		}
 		else if (splitted[i] == L"sans-serif-bold-italic")
		{
			style_.type = 4;
			style_.italic = true;
			style_.bold = true;
		}
 		else if (splitted[i] == L"monospace")
		{
			style_.type = 5;
		}
		else if (splitted[i] == L"stretched")
		{
			//style_.stretched = true;
		}
 		else if (splitted[i] == L"tailed")
		{
			//style_.type = 7;
		}
 		else if (splitted[i] == L"looped")
		{
			//style_.type = 8;
		}
 		else if (splitted[i] == L"initial")
		{
			//style_.type = 9;
		}
	}

	none_ = false;
}

math_variant math_variant::parse( const std::wstring & Value)
{
    return math_variant(Value);

}




}
}
