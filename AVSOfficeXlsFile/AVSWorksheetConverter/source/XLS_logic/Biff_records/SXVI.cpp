#include "stdafx.h"
#include "SXVI.h"

namespace XLS
{;

SXVI::SXVI()
{
}


SXVI::~SXVI()
{
}


BaseObjectPtr SXVI::clone()
{
	return BaseObjectPtr(new SXVI(*this));
}


void SXVI::writeFields(CFRecord& record)
{
#pragma message("####################### SXVI record is not implemented")
	Log::error("SXVI record is not implemented.");
	//record << some_value;
}


void SXVI::readFields(CFRecord& record)
{
#pragma message("####################### SXVI record is not implemented")
	Log::error("SXVI record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

