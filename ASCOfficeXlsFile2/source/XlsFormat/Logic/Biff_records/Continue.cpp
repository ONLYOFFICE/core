
#include "Continue.h"

namespace XLS
{

Continue::Continue()
{
}


Continue::~Continue()
{
}


BaseObjectPtr Continue::clone()
{
	return BaseObjectPtr(new Continue(*this));
}


void Continue::writeFields(CFRecord& record)
{
#pragma message("####################### Continue record is not implemented")
	Log::error("Continue record is not implemented.");
	//record << some_value;
}


void Continue::readFields(CFRecord& record)
{
#pragma message("####################### Continue record is not implemented")
	Log::error("Continue record is not implemented.");

	//record >> some_value;
}

} // namespace XLS

