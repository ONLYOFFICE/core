
#include "AxisLine.h"

namespace XLS
{;

AxisLine::AxisLine()
{
}


AxisLine::~AxisLine()
{
}


BaseObjectPtr AxisLine::clone()
{
	return BaseObjectPtr(new AxisLine(*this));
}


void AxisLine::writeFields(CFRecord& record)
{
	record << id;
}


void AxisLine::readFields(CFRecord& record)
{
	record >> id;
}

} // namespace XLS

