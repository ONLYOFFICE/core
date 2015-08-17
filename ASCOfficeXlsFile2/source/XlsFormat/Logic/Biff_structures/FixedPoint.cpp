
#include "FixedPoint.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace OSHARED
{


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

void FixedPoint::store(XLS::CFRecord& record)
{
	record << Fractional << Integral;
}


void FixedPoint::load(XLS::CFRecord& record)
{
	record >> Fractional >> Integral;
}


} // namespace OSHARED

