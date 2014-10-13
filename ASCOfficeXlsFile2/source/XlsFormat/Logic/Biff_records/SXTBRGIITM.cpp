#include "precompiled_xls.h"
#include "SXTBRGIITM.h"

namespace XLS
{;

SXTBRGIITM::SXTBRGIITM()
{
}


SXTBRGIITM::~SXTBRGIITM()
{
}


BaseObjectPtr SXTBRGIITM::clone()
{
	return BaseObjectPtr(new SXTBRGIITM(*this));
}


void SXTBRGIITM::writeFields(CFRecord& record)
{
#pragma message("####################### SXTBRGIITM record is not implemented")
	Log::error("SXTBRGIITM record is not implemented.");
	//record << some_value;
}


void SXTBRGIITM::readFields(CFRecord& record)
{
#pragma message("####################### SXTBRGIITM record is not implemented")
	Log::error("SXTBRGIITM record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

