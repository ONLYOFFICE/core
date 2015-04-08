#include "../precompiled_cpodf.h"
#include "linewidth.h"
#include "errors.h"
#include <boost_string.h>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const line_width & _Val)
{
    switch(_Val.get_type())
    {
    case line_width::Auto:
        _Wostream << "auto";
        break;
    case line_width::Normal:
        _Wostream << "normal";
        break;
    case line_width::Bold:
        _Wostream << "bold";
        break;
    case line_width::Thin:
        _Wostream << "thin";
        break;
    case line_width::Dash:
        _Wostream << "dash";
        break;
    case line_width::Medium:
        _Wostream << "medium";
        break;
    case line_width::Thick:
        _Wostream << "thick";
        break;
    case line_width::PositiveInteger:
        _Wostream << _Val.get_positive_integer();
        break;
    case line_width::Percent:
        _Wostream << _Val.get_percent();
        break;
    case line_width::PositiveLength:
        _Wostream << _Val.get_positive_length();
        break;
    }
    return _Wostream;
}

line_width::line_width(length_or_percent _Val)
{
	if (_Val.get_type() == length_or_percent::Length)
	{
		type_= PositiveLength;
		PositiveLength_ = _Val.get_length();
	}
	if (_Val.get_type() == length_or_percent::Percent)
	{
		type_= Percent;
		Percent_ = _Val.get_percent();
	}	
}

line_width line_width::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (boost::algorithm::all(tmp, boost::algorithm::is_digit()))
    {
        try 
        {
            return line_width( boost::lexical_cast<unsigned int>(tmp) );
        }
        catch(...) {}
    } 

    if (boost::algorithm::contains(tmp, L"%"))
    {
        try 
        {
            return line_width( percent::parse(tmp) );
        }
        catch(errors::invalid_attribute &) {}
    }
    
    if (tmp == L"auto")
        return line_width( Auto );
    else if (tmp == L"normal")
        return line_width( Normal );
    else if (tmp == L"bold")
        return line_width( Bold );
    else if (tmp == L"thin")
        return line_width( Thin );
    else if (tmp == L"dash")
        return line_width( Dash );
    else if (tmp == L"medium")
        return line_width( Medium );
    else if (tmp == L"thick")
        return line_width( Thick );

    try 
    {
        return line_width( length::parse(tmp) );
    } 
    catch(errors::invalid_attribute & ) {}

    BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
    return line_width(Auto);
}

} 
}

