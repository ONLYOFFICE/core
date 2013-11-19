#include "stdafx.h"
#include "SXVS.h"

namespace XLS
{;

SXVS::SXVS()
{
}


SXVS::~SXVS()
{
}


BaseObjectPtr SXVS::clone()
{
	return BaseObjectPtr(new SXVS(*this));
}


void SXVS::writeFields(CFRecord& record)
{
#pragma message("####################### SXVS record is not implemented")
	Log::error("SXVS record is not implemented.");
	//record << some_value;
}


void SXVS::readFields(CFRecord& record)
{
#pragma message("####################### SXVS record is not implemented")
	Log::error("SXVS record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

