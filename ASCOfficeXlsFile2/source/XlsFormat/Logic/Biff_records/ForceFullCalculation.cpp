#include "precompiled_xls.h"
#include "ForceFullCalculation.h"

namespace XLS
{;

ForceFullCalculation::ForceFullCalculation()
{
}


ForceFullCalculation::~ForceFullCalculation()
{
}


BaseObjectPtr ForceFullCalculation::clone()
{
	return BaseObjectPtr(new ForceFullCalculation(*this));
}


void ForceFullCalculation::writeFields(CFRecord& record)
{
#pragma message("####################### ForceFullCalculation record is not implemented")
	Log::error("ForceFullCalculation record is not implemented.");
	//record << some_value;
}


void ForceFullCalculation::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> fNoDeps;
}

} // namespace XLS

