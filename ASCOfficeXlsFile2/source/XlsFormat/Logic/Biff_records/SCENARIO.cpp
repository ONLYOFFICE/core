
#include "SCENARIO.h"

namespace XLS
{

SCENARIO::SCENARIO()
{
}


SCENARIO::~SCENARIO()
{
}


BaseObjectPtr SCENARIO::clone()
{
	return BaseObjectPtr(new SCENARIO(*this));
}


void SCENARIO::writeFields(CFRecord& record)
{
#pragma message("####################### SCENARIO record is not implemented")
	Log::error("SCENARIO record is not implemented.");
	//record << some_value;
}


void SCENARIO::readFields(CFRecord& record)
{
#pragma message("####################### SCENARIO record is not implemented")
	Log::error("SCENARIO record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

