/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "length.h"
#include "../../OOXML/Base/Base.h"

#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator<< (std::wostream & _Wostream, const length::unit _Unit)
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

std::wostream & operator<< (std::wostream & _Wostream, const length & _Length)
{
    _Wostream  << std::setprecision(4) << _Length.get_value() << _Length.get_unit();
    return _Wostream;
}
length operator+ (length _Length1, length _Length2)
{
	return length(_Length1.get_value() + _Length2.get_value(),_Length1.get_unit()); //проверка на одинаковость типа .. или приведение к одному
}

length operator+ (length _Length1, double val)
{
    return length(_Length1.get_value() + val,_Length1.get_unit());
}
length operator- (length _Length1, length _Length2)
{
	return length(_Length1.get_value() - _Length2.get_value(), _Length1.get_unit()); //проверка на одинаковость типа .. или приведение к одному
}
length operator/ (length _Length1, double val)
{
	return length(_Length1.get_value() / val, _Length1.get_unit()); 
}
length operator* (length _Length1, double val)
{
	return length(_Length1.get_value() * val, _Length1.get_unit()); 
}
bool operator== (length & _Length1, length & _Length2)
{
	if (_Length1.get_value() == _Length2.get_value())return true;
	else return false;
}
bool operator== (const length & _Length1, const length & _Length2)
{
    if (_Length1.get_value() == _Length2.get_value())return true;
    else return false;
}

length length::parse(const std::wstring & Str)
{
    int nSeparator = -1;
    int nLength = (int)Str.length();
    const wchar_t* pBuffer = Str.c_str();
    for (int i = 0; i < nLength; ++i)
    {
        if (pBuffer[i] != wchar_t('-') &&
            pBuffer[i] != wchar_t('+') &&
            pBuffer[i] != wchar_t(',') &&
            pBuffer[i] != wchar_t('.') &&
            (pBuffer[i] < wchar_t('0') || pBuffer[i] > wchar_t('9')))
        {
            nSeparator = i;
            break;
        }
    }

    //double v = (nSeparator > 0) ? std::stod(Str.substr(0, nSeparator)) : 0;
    // вообще предыдущая строчка лучше намного, но сделаю как было (без c++11)
    double v = 0;
    if (nSeparator > 0)
    {
        std::wstringstream strm;
        strm << Str.substr(0, nSeparator);
        strm >> v;
    }

    std::wstring uStr = (-1 != nSeparator) ? Str.substr(nSeparator) : L"";

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
    return Val / 12700;	// 1 / 12700  =  72.0 / (360000.0 * 2.54);
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
double pt_to_mm(double Val)
{
    return Val / 2.834467120181406;
    
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

_INT64 pt_to_emu(double Val)
{
    return static_cast<_INT64>(Val * 360000 * 2.54) / 72;
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
    else if (Unit == mm)
    {
        return pt_to_mm( to_pt(*this) );
    }
	else
    {
        return 0.0;
    }
}
//--------------------------------------------------------------------
std::wostream & operator<< (std::wostream & _Wostream, const vector3D & _vector3D)
{
	_Wostream  << L"(" << _vector3D.get_x() << L" " << _vector3D.get_y() << L" " << _vector3D.get_z() << L")";
    return _Wostream;
}
vector3D vector3D::parse(const std::wstring & Str)
{
	if (Str.length() < 8) return vector3D();

	std::wstring vsp = Str.substr(1, Str.length() - 2);
	
	std::vector<std::wstring> coord;
	boost::algorithm::split(coord, vsp, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

	if (coord.size() != 3) return vector3D();
 
    return vector3D(std::stod(coord[0]), std::stod(coord[1]), std::stod(coord[2]));
}


} }

