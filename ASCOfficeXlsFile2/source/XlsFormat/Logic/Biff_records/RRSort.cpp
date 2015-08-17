
#include "RRSort.h"

namespace XLS
{

RRSort::RRSort()
{
}


RRSort::~RRSort()
{
}


BaseObjectPtr RRSort::clone()
{
	return BaseObjectPtr(new RRSort(*this));
}


void RRSort::writeFields(CFRecord& record)
{
#pragma message("####################### RRSort record is not implemented")
	Log::error("RRSort record is not implemented.");
	//record << some_value;
}


void RRSort::readFields(CFRecord& record)
{
#pragma message("####################### RRSort record is not implemented")
	Log::error("RRSort record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

