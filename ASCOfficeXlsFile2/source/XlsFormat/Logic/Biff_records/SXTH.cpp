
#include "SXTH.h"

namespace XLS
{

SXTH::SXTH()
{
}


SXTH::~SXTH()
{
}


BaseObjectPtr SXTH::clone()
{
	return BaseObjectPtr(new SXTH(*this));
}


void SXTH::writeFields(CFRecord& record)
{
#pragma message("####################### SXTH record is not implemented")
	Log::error("SXTH record is not implemented.");
	//record << some_value;
}


void SXTH::readFields(CFRecord& record)
{
#pragma message("####################### SXTH record is not implemented")
	Log::error("SXTH record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

