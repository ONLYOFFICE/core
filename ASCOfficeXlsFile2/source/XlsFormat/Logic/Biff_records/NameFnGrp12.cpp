
#include "NameFnGrp12.h"

namespace XLS
{

NameFnGrp12::NameFnGrp12()
{
}


NameFnGrp12::~NameFnGrp12()
{
}


BaseObjectPtr NameFnGrp12::clone()
{
	return BaseObjectPtr(new NameFnGrp12(*this));
}


void NameFnGrp12::writeFields(CFRecord& record)
{
#pragma message("####################### NameFnGrp12 record is not implemented")
	Log::error("NameFnGrp12 record is not implemented.");
	//record << some_value;
}


void NameFnGrp12::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> cachName >> fgrp >> rgach;
}

} // namespace XLS

