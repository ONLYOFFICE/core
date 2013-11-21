#include "stdafx.h"
#include "FnGrp12.h"

namespace XLS
{;

FnGrp12::FnGrp12()
{
}


FnGrp12::~FnGrp12()
{
}


BaseObjectPtr FnGrp12::clone()
{
	return BaseObjectPtr(new FnGrp12(*this));
}


void FnGrp12::writeFields(CFRecord& record)
{
#pragma message("####################### FnGrp12 record is not implemented")
	Log::error("FnGrp12 record is not implemented.");
	//record << some_value;
}


void FnGrp12::readFields(CFRecord& record)
{
	record.skipNBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> astFnGrp;
}

} // namespace XLS

