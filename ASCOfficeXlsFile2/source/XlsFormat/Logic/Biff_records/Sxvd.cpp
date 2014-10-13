#include "precompiled_xls.h"
#include "Sxvd.h"

namespace XLS
{;

Sxvd::Sxvd()
{
}


Sxvd::~Sxvd()
{
}


BaseObjectPtr Sxvd::clone()
{
	return BaseObjectPtr(new Sxvd(*this));
}


void Sxvd::writeFields(CFRecord& record)
{
#pragma message("####################### Sxvd record is not implemented")
	Log::error("Sxvd record is not implemented.");
	//record << some_value;
}


void Sxvd::readFields(CFRecord& record)
{
#pragma message("####################### Sxvd record is not implemented")
	Log::error("Sxvd record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

