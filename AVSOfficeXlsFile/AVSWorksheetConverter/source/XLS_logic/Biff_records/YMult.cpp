#include "stdafx.h"
#include "YMult.h"

namespace XLS
{;

YMult::YMult()
{
}


YMult::~YMult()
{
}


BaseObjectPtr YMult::clone()
{
	return BaseObjectPtr(new YMult(*this));
}


void YMult::writeFields(CFRecord& record)
{
#pragma message("####################### YMult record is not implemented")
	Log::error("YMult record is not implemented.");
	//record << some_value;
}


void YMult::readFields(CFRecord& record)
{
#pragma message("####################### YMult record is not implemented")
	Log::error("YMult record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

