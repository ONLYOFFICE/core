#include "precompiled_cpodf.h"
#include "xlink.h"

#include <ostream>

namespace cpdoccore {
namespace odf {

std::wostream & operator << (std::wostream & _Wostream, const xlink_type & _Type)
{
    switch (_Type.get_type())
    {
    default:
    case xlink_type::Simple:
        _Wostream << L"simple";
        break;
    }
    return _Wostream;
}

std::wostream & operator << (std::wostream & _Wostream, const xlink_actuate & _Actuate)
{
    switch (_Actuate.get_type())
    {
    default:
    case xlink_actuate::OnRequest:
        _Wostream << "onRequest";
        break;
    }
    return _Wostream;
}

std::wostream & operator << (std::wostream & _Wostream, const xlink_show & _Show)
{
    switch (_Show.get_type())
    {
    default:
    case xlink_show::New:
        _Wostream << L"new";
        break;
    case xlink_show::Replace:
        _Wostream << L"replace";
        break;
    }
    return _Wostream;
}

xlink_type xlink_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"simple")
        return xlink_type( Simple );
    else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return xlink_type( Simple );
    }
}

xlink_actuate xlink_actuate::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"onrequest")
        return xlink_actuate( OnRequest );
    else if (tmp == L"onload")
        return xlink_actuate( OnLoad );   
	else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return xlink_actuate( OnRequest );
    }
}

xlink_show xlink_show::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"new")
        return xlink_show( New );
    else if (tmp == L"replace")
        return xlink_show( Replace );
     else if (tmp == L"embed")
        return xlink_show( Embed );
	else
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );        
        return xlink_show( New );
    }
}

}
}
