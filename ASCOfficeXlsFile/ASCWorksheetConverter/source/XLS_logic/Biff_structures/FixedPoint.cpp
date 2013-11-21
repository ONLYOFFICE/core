#include "stdafx.h"
#include "FixedPoint.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/AttributeDataWrong.h>

namespace OSHARED
{;


FixedPoint::FixedPoint()
{
}


FixedPoint::FixedPoint(const long raw_data)
{
	Fractional = static_cast<BYTE>(raw_data & 0xFFFF);
	Integral = static_cast<BYTE>(raw_data >> 16);
}


XLS::BiffStructurePtr FixedPoint::clone()
{
	return XLS::BiffStructurePtr(new FixedPoint(*this));
}


void FixedPoint::toXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	parent->setAttribute(attrib_name, static_cast<double>(Integral) + (static_cast<double>(Fractional) / 65536.0));
}


const bool FixedPoint::fromXML(MSXML2::IXMLDOMElementPtr parent, const _bstr_t& attrib_name)
{
	_variant_t raw_value = getStructAttribute(parent, attrib_name);
	double real_value = raw_value;
	if(real_value > MAXSHORT || real_value < MINSHORT)
	{
		EXCEPT::LE::AttributeDataWrong(attrib_name, parent->GetnodeName(), static_cast<_bstr_t>(raw_value));
	}
	Integral = static_cast<short>(real_value);
	Fractional = static_cast<WORD>((real_value - static_cast<double>(Integral)) * 65536.0);
	return true;
}


void FixedPoint::store(XLS::CFRecord& record)
{
	record << Fractional << Integral;
}


void FixedPoint::load(XLS::CFRecord& record)
{
	record >> Fractional >> Integral;
}


} // namespace OSHARED

