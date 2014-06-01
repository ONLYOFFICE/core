#include "precompiled_cpodf.h"
#include "calcext_type.h"

#include <ostream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const calcext_type & _Val)
{
    switch(_Val.get_type())
    {
    case calcext_type::Percent:
        _Wostream << L"percent";
		break;
	case calcext_type::Number:
        _Wostream << L"number";
        break;
    case calcext_type::AutoMaximum:
        _Wostream << L"auto-maximum";
        break;
    case calcext_type::AutoMinimum:
        _Wostream << L"auto-minimum";
        break;
    case calcext_type::Maximum:
        _Wostream << L"maximum";
        break;
    case calcext_type::Minimum:
        _Wostream << L"minimum";
        break;
    default:
        break;
    }
    return _Wostream;    
}

calcext_type calcext_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"auto-maximum")
        return calcext_type( AutoMaximum );
    else if (tmp == L"auto-minimum")
        return calcext_type( AutoMinimum );
    else if (tmp == L"number")
        return calcext_type( Number );
    else if (tmp == L"percent")
        return calcext_type( Percent );
    else if (tmp == L"maximum")
        return calcext_type( Maximum );
    else if (tmp == L"minimum")
        return calcext_type( Minimum );
	else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return calcext_type( Number );
    }
}

} }
