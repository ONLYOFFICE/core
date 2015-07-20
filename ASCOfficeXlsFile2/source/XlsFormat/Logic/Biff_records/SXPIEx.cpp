
#include "SXPIEx.h"

namespace XLS
{;

SXPIEx::SXPIEx()
{
}


SXPIEx::~SXPIEx()
{
}


BaseObjectPtr SXPIEx::clone()
{
	return BaseObjectPtr(new SXPIEx(*this));
}


void SXPIEx::writeFields(CFRecord& record)
{
#pragma message("####################### SXPIEx record is not implemented")
	Log::error("SXPIEx record is not implemented.");
	//record << some_value;
}


void SXPIEx::readFields(CFRecord& record)
{
#pragma message("####################### SXPIEx record is not implemented")
	Log::error("SXPIEx record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

