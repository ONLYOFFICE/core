#include "precompiled_xls.h"
#include "FixedPoint.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace OSHARED
{;


FixedPoint::FixedPoint()
{
}


FixedPoint::FixedPoint(const long raw_data)
{
	Fractional = static_cast<unsigned char>(raw_data & 0xFFFF);
	Integral = static_cast<unsigned char>(raw_data >> 16);
}


XLS::BiffStructurePtr FixedPoint::clone()
{
	return XLS::BiffStructurePtr(new FixedPoint(*this));
}


//void FixedPoint::toXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	parent->setAttribute(attrib_name, static_cast<double>(Integral) + (static_cast<double>(Fractional) / 65536.0));
//}
//
//
//const bool FixedPoint::fromXML(BiffStructurePtr & parent, const std::wstring & attrib_name)
//{
//	_variant_t raw_value = getStructAttribute(parent, attrib_name);
//	double real_value = raw_value;
//	if(real_value > MAXSHORT || real_value < MINSHORT)
//	{
//		EXCEPT::LE::AttributeDataWrong(attrib_name, parent->GetnodeName(), static_cast<std::wstring >(raw_value));
//	}
//	Integral = static_cast<short>(real_value);
//	Fractional = static_cast<unsigned short>((real_value - static_cast<double>(Integral)) * 65536.0);
//	return true;
//}
//
//
void FixedPoint::store(XLS::CFRecord& record)
{
	record << Fractional << Integral;
}


void FixedPoint::load(XLS::CFRecord& record)
{
	record >> Fractional >> Integral;
}


} // namespace OSHARED

