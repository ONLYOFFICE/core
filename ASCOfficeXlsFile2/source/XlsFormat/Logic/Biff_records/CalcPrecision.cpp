#include "precompiled_xls.h"
#include "CalcPrecision.h"

namespace XLS
{;

CalcPrecision::CalcPrecision()
{
}


CalcPrecision::~CalcPrecision()
{
}


BaseObjectPtr CalcPrecision::clone()
{
	return BaseObjectPtr(new CalcPrecision(*this));
}


void CalcPrecision::writeFields(CFRecord& record)
{
	record << fFullPrec;
}


void CalcPrecision::readFields(CFRecord& record)
{
	record >> fFullPrec;
}

} // namespace XLS

