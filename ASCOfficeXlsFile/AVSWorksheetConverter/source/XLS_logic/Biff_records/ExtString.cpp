#include "stdafx.h"
#include "ExtString.h"

namespace XLS
{;

ExtString::ExtString()
{
}


ExtString::~ExtString()
{
}


BaseObjectPtr ExtString::clone()
{
	return BaseObjectPtr(new ExtString(*this));
}


void ExtString::writeFields(CFRecord& record)
{
#pragma message("####################### ExtString record is not implemented")
	Log::error("ExtString record is not implemented.");
	//record << some_value;
}


void ExtString::readFields(CFRecord& record)
{
#pragma message("####################### ExtString record is not implemented")
	Log::error("ExtString record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

