
#include "length.h"

#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const length::unit _Unit)
{
    switch(_Unit)
    {
    case length::cm:
        _Wostream << L"cm";
        break;
    case length::mm:
        _Wostream << L"mm";
        break;
    case length::px:
        _Wostream << L"px";
        break;
    case length::inch:
        _Wostream << L"inch";
        break;
    case length::pt:
        _Wostream << L"pt";
        break;
    case length::rel:
        _Wostream << L"*";
        break;
    case length::emu:
        _Wostream << L"emu";
        break;
    default:
    case length::none:
        _Wostream << L"NONE";
        break;
    }
    return _Wostream;
}

std::wostream & operator << (std::wostream & _Wostream, const length & _Length)
{
    _Wostream  << std::setprecision(4) << _Length.get_value() << _Length.get_unit();
    return _Wostream;
}
length operator + (length & _Length1, length & _Length2)
{
	return length(_Length1.get_value() + _Length2.get_value(),_Length1.get_unit()); //проверка на одинаковость типа .. или приведение к одному
}
length operator - (length & _Length1, length & _Length2)
{
	return length(_Length1.get_value() - _Length2.get_value(),_Length1.get_unit()); //проверка на одинаковость типа .. или приведение к одному
}
length operator / (length & _Length1, double val)
{
	return length(_Length1.get_value() / val ,_Length1.get_unit()); 
}
length operator * (length & _Length1, double val)
{
	return length(_Length1.get_value() * val ,_Length1.get_unit()); 
}
bool operator == (length & _Length1, length & _Length2)
{
	if (_Length1.get_value() == _Length2.get_value())return true;
	else return false;
}

length length::parse(const std::wstring & Str)
{
    double v;
    std::wstring uStr;
    std::wstringstream strm;
    strm << Str;
    if (!(strm >> v && strm >> uStr) || (strm.get() != std::wstringstream::traits_type::eof()))
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );
    }

    length::unit u = length::none;

    if (uStr == L"cm")
        u = length::cm;
    else if (uStr == L"mm")
        u = length::mm;
    else if (uStr == L"px")
        u = length::px;
    else if (uStr == L"inch")
        u = length::inch;
    else if (uStr == L"in")
        u = length::inch;
    else if (uStr == L"pt")
        u = length::pt;
    else if (uStr == L"*")
        u = length::rel;
    else if (uStr == L"emu")
        u = length::emu;
    else
    {
       // BOOST_THROW_EXCEPTION( errors::invalid_attribute() );    
    }

    return length(v, u);
}

namespace {

double cm_to_pt(double Val)
{
    return 28.34467120181406 * Val;
}

double mm_to_pt(double Val)
{
    return 2.834467120181406 * Val;
}

double inch_to_pt(double Val)
{
    return 72.0 * Val;
}

double emu_to_pt(double Val)
{
    return Val * 72.0 / (360000.0 * 2.54);
}
double px_to_pt(double Val)
{
    return Val * 3 / 4;
}
double pt_to_inch(double Val)
{
    return Val / 72.0;
}

double pt_to_cm(double Val)
{
    return Val / 28.34467120181406;
    
}

double to_pt(const length & Val)
{
    switch(Val.get_unit())
    {
    case length::cm:
        return cm_to_pt(Val.get_value());
    case length::mm:
        return mm_to_pt(Val.get_value());
    case length::inch:
        return inch_to_pt(Val.get_value());
    case length::pt:
        return Val.get_value();
    case length::emu:
        return emu_to_pt(Val.get_value());
	case length::px:
		return px_to_pt(Val.get_value());
    }
    //CP_ASSERT(false);
    return 0.0;
}

int pt_to_emu(double Val)
{
    return static_cast<int>(Val * 360000 * 2.54) / 72;
}

}

double length::get_value_unit(unit Unit) const
{
    if (Unit == pt)
    {
        return to_pt(*this);
    } 
    else if (Unit == emu)
    {
        return pt_to_emu( to_pt(*this) );    
    } 
    else if (Unit == inch)
    {
        return pt_to_inch( to_pt(*this) );
    }
    else if (Unit == cm)
    {
        return pt_to_cm( to_pt(*this) );
    }
    else
    {
        return 0.0;
    }
}

} }

