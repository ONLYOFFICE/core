#include "precompiled_xls.h"
#include "CalcSaveRecalc.h"

namespace XLS
{;

CalcSaveRecalc::CalcSaveRecalc()
{
}


CalcSaveRecalc::~CalcSaveRecalc()
{
}


BaseObjectPtr CalcSaveRecalc::clone()
{
	return BaseObjectPtr(new CalcSaveRecalc(*this));
}


void CalcSaveRecalc::writeFields(CFRecord& record)
{
	record << fSaveRecalc;
}


void CalcSaveRecalc::readFields(CFRecord& record)
{
	record >> fSaveRecalc;
}

} // namespace XLS

