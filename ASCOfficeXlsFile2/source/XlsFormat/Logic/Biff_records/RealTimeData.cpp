
#include "RealTimeData.h"

namespace XLS
{;

RealTimeData::RealTimeData()
{
}


RealTimeData::~RealTimeData()
{
}


BaseObjectPtr RealTimeData::clone()
{
	return BaseObjectPtr(new RealTimeData(*this));
}


void RealTimeData::writeFields(CFRecord& record)
{
#pragma message("####################### RealTimeData record is not implemented")
	Log::error("RealTimeData record is not implemented.");
	//record << some_value;
}


void RealTimeData::readFields(CFRecord& record)
{
#pragma message("####################### RealTimeData record is not implemented")
	Log::error("RealTimeData record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

