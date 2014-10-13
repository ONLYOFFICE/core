#include "precompiled_xls.h"
#include "FilterMode.h"

namespace XLS
{;

FilterMode::FilterMode()
{
}


FilterMode::~FilterMode()
{
}


BaseObjectPtr FilterMode::clone()
{
	return BaseObjectPtr(new FilterMode(*this));
}


void FilterMode::writeFields(CFRecord& record)
{
#pragma message("####################### FilterMode record is not implemented")
	Log::error("FilterMode record is not implemented.");
	//record << some_value;
}


void FilterMode::readFields(CFRecord& record)
{
	// No data
}

} // namespace XLS

