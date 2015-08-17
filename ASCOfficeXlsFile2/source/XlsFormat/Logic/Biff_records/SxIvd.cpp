
#include "SxIvd.h"

namespace XLS
{

SxIvd::SxIvd()
{
}


SxIvd::~SxIvd()
{
}


BaseObjectPtr SxIvd::clone()
{
	return BaseObjectPtr(new SxIvd(*this));
}


void SxIvd::writeFields(CFRecord& record)
{
#pragma message("####################### SxIvd record is not implemented")
	Log::error("SxIvd record is not implemented.");
	//record << some_value;
}


void SxIvd::readFields(CFRecord& record)
{
#pragma message("####################### SxIvd record is not implemented")
	Log::error("SxIvd record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

