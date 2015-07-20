
#include "PivotChartBits.h"

namespace XLS
{;

PivotChartBits::PivotChartBits()
{
}


PivotChartBits::~PivotChartBits()
{
}


BaseObjectPtr PivotChartBits::clone()
{
	return BaseObjectPtr(new PivotChartBits(*this));
}


void PivotChartBits::writeFields(CFRecord& record)
{
#pragma message("####################### PivotChartBits record is not implemented")
	Log::error("PivotChartBits record is not implemented.");
	//record << some_value;
}


void PivotChartBits::readFields(CFRecord& record)
{
#pragma message("####################### PivotChartBits record is not implemented")
	Log::error("PivotChartBits record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

