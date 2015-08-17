
#include "AxesUsed.h"

namespace XLS
{

AxesUsed::AxesUsed()
{
}


AxesUsed::~AxesUsed()
{
}


BaseObjectPtr AxesUsed::clone()
{
	return BaseObjectPtr(new AxesUsed(*this));
}


void AxesUsed::writeFields(CFRecord& record)
{
	record << cAxes;
}


void AxesUsed::readFields(CFRecord& record)
{
	record >> cAxes;
}

} // namespace XLS

