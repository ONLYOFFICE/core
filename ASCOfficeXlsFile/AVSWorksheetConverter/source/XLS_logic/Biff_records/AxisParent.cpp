#include "stdafx.h"
#include "AxisParent.h"

namespace XLS
{;

AxisParent::AxisParent()
{
}


AxisParent::~AxisParent()
{
}


BaseObjectPtr AxisParent::clone()
{
	return BaseObjectPtr(new AxisParent(*this));
}


void AxisParent::writeFields(CFRecord& record)
{
	record << iax;
	record.reserveNBytes(16);// unused
}


void AxisParent::readFields(CFRecord& record)
{
	record >> iax;
	record.skipNBytes(16); // unused
}



} // namespace XLS

