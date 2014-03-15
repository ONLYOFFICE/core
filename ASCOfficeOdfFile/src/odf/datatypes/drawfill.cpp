#include "precompiled_cpodf.h"
#include "drawfill.h"

#include <ostream>

namespace cpdoccore { 
namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const draw_fill & _Val)
{
	std::wstring fillType;
	switch(_Val.get_type())
	{
		case draw_fill::none:		_Wostream << L"none" ;break;
		case draw_fill::hatch:		_Wostream << L"hatch" ;break;
		case draw_fill::solid:		_Wostream << L"solid" ;break;
		case draw_fill::gradient:	_Wostream << L"gradient" ;break;
		case draw_fill::bitmap:		_Wostream << L"bitmap" ;break;
	}

    return _Wostream;
}
//std::wstring draw_fill::get_type_ms()
//{
//	std::wstring fillType;
//	switch(type_)
//	{
//		case draw_fill::none:		fillType = L"a:noFill";break;
//		case draw_fill::hatch:		fillType = L"a:pattFill";break;
//		case draw_fill::solid:		fillType = L"a:solidFill";break;
//		case draw_fill::gradient:	fillType = L"a:gradFill";break;
//		case draw_fill::bitmap:		fillType = L"a:imageFill";break;
//	}
//	return fillType;
//}
draw_fill draw_fill::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"none")
        return draw_fill( none );
    else if (tmp == L"solid")
        return draw_fill( solid );
    else if (tmp == L"bitmap")
        return draw_fill( bitmap );
    else if (tmp == L"gradient")
        return draw_fill( gradient );
    else if (tmp == L"hatch")
        return draw_fill( hatch );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return draw_fill( solid );
    }
}
}
}
