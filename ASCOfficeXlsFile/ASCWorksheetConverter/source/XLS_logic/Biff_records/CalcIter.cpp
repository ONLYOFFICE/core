#include "stdafx.h"
#include "CalcIter.h"

namespace XLS
{;

CalcIter::CalcIter()
{
}


CalcIter::~CalcIter()
{
}


BaseObjectPtr CalcIter::clone()
{
	return BaseObjectPtr(new CalcIter(*this));
}


void CalcIter::writeFields(CFRecord& record)
{
	record << vfIter;
}


void CalcIter::readFields(CFRecord& record)
{
	record >> vfIter;
}

} // namespace XLS

