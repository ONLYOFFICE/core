
#include "MTRSettings.h"

namespace XLS
{;

MTRSettings::MTRSettings()
{
}


MTRSettings::~MTRSettings()
{
}


BaseObjectPtr MTRSettings::clone()
{
	return BaseObjectPtr(new MTRSettings(*this));
}


void MTRSettings::writeFields(CFRecord& record)
{
#pragma message("####################### MTRSettings record is not implemented")
	Log::error("MTRSettings record is not implemented.");
	//record << some_value;
}


void MTRSettings::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> fMTREnabled >> fUserSetThreadCount >> cUserThreadCount;
}

} // namespace XLS

