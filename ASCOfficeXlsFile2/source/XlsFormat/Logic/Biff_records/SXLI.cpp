#include "precompiled_xls.h"
#include "SXLI.h"

namespace XLS
{;

SXLI::SXLI()
{
}


SXLI::~SXLI()
{
}


BaseObjectPtr SXLI::clone()
{
	return BaseObjectPtr(new SXLI(*this));
}


void SXLI::writeFields(CFRecord& record)
{
#pragma message("####################### SXLI record is not implemented")
	Log::error("SXLI record is not implemented.");
	//record << some_value;
}


void SXLI::readFields(CFRecord& record)
{
#pragma message("####################### SXLI record is not implemented")
	Log::error("SXLI record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

