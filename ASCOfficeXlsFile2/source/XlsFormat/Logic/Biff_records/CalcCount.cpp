#include "precompiled_xls.h"
#include "CalcCount.h"

namespace XLS
{;

CalcCount::CalcCount()
{
}


CalcCount::~CalcCount()
{
}


BaseObjectPtr CalcCount::clone()
{
	return BaseObjectPtr(new CalcCount(*this));
}


void CalcCount::writeFields(CFRecord& record)
{
	record << cIter;
}


void CalcCount::readFields(CFRecord& record)
{
	record >> cIter;
}

} // namespace XLS

