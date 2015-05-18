
#include "percent.h"
#include <iostream>
#include <sstream>
#include "errors.h"

namespace cpdoccore { namespace odf { 

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

}
}

