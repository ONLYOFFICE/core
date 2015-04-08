#include "../precompiled_cpodf.h"
#include "iconset_type.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const iconset_type & _Val)
{
    switch(_Val.get_type())
    {
    case iconset_type::Arrows3:
        _Wostream << L"3Arrows";
        break;
    case iconset_type::Arrows3Gray:
        _Wostream << L"3ArrowsGray";
        break;
    case iconset_type::Flags3:
        _Wostream << L"3Flags";
        break;
 	case iconset_type::Signs3:
        _Wostream << L"3Signs";
        break;
	case iconset_type::Symbols3:
        _Wostream << L"3Symbols";
        break;
	case iconset_type::Symbols3_2:
        _Wostream << L"3Symbols2";
        break;    
	case iconset_type::Traffic3Lights1:
        _Wostream << L"3TrafficLights1";
        break;
	case iconset_type::Traffic3Lights2:
        _Wostream << L"3TrafficLights2";
        break;
    case iconset_type::Arrows4:
        _Wostream << L"4Arrows";
        break;
    case iconset_type::Arrows4Gray:
        _Wostream << L"4ArrowsGray";
        break;
	case iconset_type::Rating4:
        _Wostream << L"4Rating";
        break;
	case iconset_type::RedToBlack4:
        _Wostream << L"4RedToBlack";
        break;
	case iconset_type::Traffic4Lights:
        _Wostream << L"4TrafficLights";
		break;
    case iconset_type::Arrows5:
        _Wostream << L"5Arrows";
        break;
    case iconset_type::Arrows5Gray:
        _Wostream << L"5ArrowsGray";
        break;
	case iconset_type::Quarters5:
        _Wostream << L"5Quarters";
        break;
	case iconset_type::Rating5:
        _Wostream << L"5Rating";
        break;
	 default:
        break;
    }
    return _Wostream;    
}

iconset_type iconset_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"3Arrows")
        return iconset_type( Arrows3 );
	else     if (tmp == L"3ArrowsGray")
        return iconset_type( Arrows3Gray );
	else     if (tmp == L"3Flags")
        return iconset_type( Flags3 );
	else     if (tmp == L"3Signs")
        return iconset_type( Signs3 );
	else     if (tmp == L"3Symbols")
        return iconset_type( Symbols3 );
	else	if (tmp == L"3Symbols2")
        return iconset_type( Symbols3_2 );
	else     if (tmp == L"3TrafficLights1")
        return iconset_type( Traffic3Lights1 );
    else	if (tmp == L"3TrafficLights2")
        return iconset_type( Traffic3Lights2 );
    else	if (tmp == L"4Arrows")
        return iconset_type( Arrows4 );
	else	if (tmp == L"4ArrowsGray")
        return iconset_type( Arrows4Gray );
	else	if (tmp == L"4Rating")
        return iconset_type( Rating4 );
	else	if (tmp == L"4RedToBlack")
        return iconset_type( RedToBlack4 );
	else	if (tmp == L"4TrafficLights")
        return iconset_type( Traffic4Lights );
	else	if (tmp == L"5Arrows")
        return iconset_type( Arrows5 );
	else    if (tmp == L"5ArrowsGray")
        return iconset_type( Arrows5Gray );
	else	if (tmp == L"5Quarters")
        return iconset_type( Quarters5 );
	else	if (tmp == L"5Rating")
        return iconset_type( Rating5 );
	else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return iconset_type( Arrows3 );
    }
}

} }
