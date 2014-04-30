#include "precompiled_cpodf.h"
#include "fillimagerefpoint.h"

#include <ostream>

namespace cpdoccore { 
namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const fill_image_ref_point & _Val)
{
	std::wstring fillType;
	switch(_Val.get_type())
	{
		case fill_image_ref_point::top_left:	_Wostream << L"top-left" ;break;
		case fill_image_ref_point::top:			_Wostream << L"top" ;break;
		case fill_image_ref_point::top_right:	_Wostream << L"top-right" ;break;
		case fill_image_ref_point::left:		_Wostream << L"left" ;break;
		case fill_image_ref_point::center:		_Wostream << L"center" ;break;
		case fill_image_ref_point::right:		_Wostream << L"right" ;break;
		case fill_image_ref_point::bottom_left:	_Wostream << L"bottom-left" ;break;
		case fill_image_ref_point::bottom:		_Wostream << L"bottom" ;break;
		case fill_image_ref_point::bottom_right:_Wostream << L"bottom-right" ;break;
	}

    return _Wostream;
}

fill_image_ref_point fill_image_ref_point::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"left")
        return fill_image_ref_point( left );
    else if (tmp == L"top")
        return fill_image_ref_point( top );
     else if (tmp == L"top-left")
        return fill_image_ref_point( top_left );
    else if (tmp == L"top-right")
        return fill_image_ref_point( top_right );
	else if (tmp == L"center")
        return fill_image_ref_point( center );
    else if (tmp == L"right")
        return fill_image_ref_point(right );
    else if (tmp == L"bottom-left")
        return fill_image_ref_point( bottom_left );
    else if (tmp == L"bottom")
        return fill_image_ref_point( bottom );
    else if (tmp == L"bottom-right")
        return fill_image_ref_point( bottom_right );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return fill_image_ref_point( center );
    }
}
}
}
