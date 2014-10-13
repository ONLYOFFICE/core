#include "precompiled_xls.h"
#include "WOpt.h"

namespace XLS
{;

WOpt::WOpt()
{
}


WOpt::~WOpt()
{
}


BaseObjectPtr WOpt::clone()
{
	return BaseObjectPtr(new WOpt(*this));
}


void WOpt::writeFields(CFRecord& record)
{
#pragma message("####################### WOpt record is not implemented")
	Log::error("WOpt record is not implemented.");
	//record << some_value;
}


void WOpt::readFields(CFRecord& record)
{
#pragma message("####################### WOpt record is not implemented")
	Log::error("WOpt record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

