
#include "LegendException.h"

namespace XLS
{

LegendException::LegendException()
{
}


LegendException::~LegendException()
{
}


BaseObjectPtr LegendException::clone()
{
	return BaseObjectPtr(new LegendException(*this));
}


void LegendException::writeFields(CFRecord& record)
{
#pragma message("####################### LegendException record is not implemented")
	Log::error("LegendException record is not implemented.");
	//record << some_value;
}


void LegendException::readFields(CFRecord& record)
{
#pragma message("####################### LegendException record is not implemented")
	Log::error("LegendException record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

