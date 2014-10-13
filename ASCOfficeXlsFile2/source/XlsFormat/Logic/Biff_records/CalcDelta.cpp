#include "precompiled_xls.h"
#include "CalcDelta.h"

namespace XLS
{;

CalcDelta::CalcDelta()
{
}


CalcDelta::~CalcDelta()
{
}


BaseObjectPtr CalcDelta::clone()
{
	return BaseObjectPtr(new CalcDelta(*this));
}


void CalcDelta::writeFields(CFRecord& record)
{
	record << numDelta;
}


void CalcDelta::readFields(CFRecord& record)
{
	record >> numDelta;
}

} // namespace XLS

