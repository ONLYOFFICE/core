
#include "percent.h"
#include <iostream>
#include <sstream>
#include "errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const percent & _Percent)
{
    _Wostream << _Percent.get_value() << L"%";
    return _Wostream;
}

std::wistream & operator >> (std::wistream & _Wistream, percent & _Percent)
{
    wchar_t p;
    double val;
    _Wistream >> val >> p;
    if (p != L'%')
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );    
    }

    _Percent = percent(val);
    return _Wistream;
}

percent percent::parse(const std::wstring & Str)
{
    percent per_;
    std::wstringstream strm;
    strm << Str;
    strm >> per_;
    return per_;
}
//----------------------------------------------------------------

std::wostream & operator << (std::wostream & _Wostream, const percent_rel & _percent_rel)
{
	if (_percent_rel.get_value()  >= 0)
		_Wostream << _percent_rel.get_value() << L"*";
	else 
		_Wostream << L"-1*";
    return _Wostream;
}

std::wistream & operator >> (std::wistream & _Wistream, percent_rel & _percent_rel)
{
    wchar_t p;
    int val;
    _Wistream >> val >> p;
   
	if (p != L'*')
        _percent_rel = percent_rel(0);   
	else
		_percent_rel = percent_rel(val);
    return _Wistream;
}

percent_rel percent_rel::parse(const std::wstring & Str)
{
    percent_rel per_(0);
    std::wstringstream strm;
    strm << Str;
    strm >> per_;
    return per_;
}


}
}

