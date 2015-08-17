
#include "End.h"

namespace XLS
{

End::End()
{
}


End::~End()
{
}


BaseObjectPtr End::clone()
{
	return BaseObjectPtr(new End(*this));
}


void End::writeFields(CFRecord& record)
{
#pragma message("####################### End record is not implemented")
	Log::error("End record is not implemented.");
	//record << some_value;
}


void End::readFields(CFRecord& record)
{
	// no data
}

} // namespace XLS

