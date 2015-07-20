
#include "SXDI.h"

namespace XLS
{;

SXDI::SXDI()
{
}


SXDI::~SXDI()
{
}


BaseObjectPtr SXDI::clone()
{
	return BaseObjectPtr(new SXDI(*this));
}


void SXDI::writeFields(CFRecord& record)
{
#pragma message("####################### SXDI record is not implemented")
	Log::error("SXDI record is not implemented.");
	//record << some_value;
}


void SXDI::readFields(CFRecord& record)
{
#pragma message("####################### SXDI record is not implemented")
	Log::error("SXDI record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

