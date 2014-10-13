#include "precompiled_xls.h"
#include "SxFilt.h"

namespace XLS
{;

SxFilt::SxFilt()
{
}


SxFilt::~SxFilt()
{
}


BaseObjectPtr SxFilt::clone()
{
	return BaseObjectPtr(new SxFilt(*this));
}


void SxFilt::writeFields(CFRecord& record)
{
#pragma message("####################### SxFilt record is not implemented")
	Log::error("SxFilt record is not implemented.");
	//record << some_value;
}


void SxFilt::readFields(CFRecord& record)
{
#pragma message("####################### SxFilt record is not implemented")
	Log::error("SxFilt record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

