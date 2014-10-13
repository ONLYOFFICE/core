#include "precompiled_xls.h"
#include "SXEx.h"

namespace XLS
{;

SXEx::SXEx()
{
}


SXEx::~SXEx()
{
}


BaseObjectPtr SXEx::clone()
{
	return BaseObjectPtr(new SXEx(*this));
}


void SXEx::writeFields(CFRecord& record)
{
#pragma message("####################### SXEx record is not implemented")
	Log::error("SXEx record is not implemented.");
	//record << some_value;
}


void SXEx::readFields(CFRecord& record)
{
#pragma message("####################### SXEx record is not implemented")
	Log::error("SXEx record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

