#include "stdafx.h"
#include "SXPI.h"

namespace XLS
{;

SXPI::SXPI()
{
}


SXPI::~SXPI()
{
}


BaseObjectPtr SXPI::clone()
{
	return BaseObjectPtr(new SXPI(*this));
}


void SXPI::writeFields(CFRecord& record)
{
#pragma message("####################### SXPI record is not implemented")
	Log::error("SXPI record is not implemented.");
	//record << some_value;
}


void SXPI::readFields(CFRecord& record)
{
#pragma message("####################### SXPI record is not implemented")
	Log::error("SXPI record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

