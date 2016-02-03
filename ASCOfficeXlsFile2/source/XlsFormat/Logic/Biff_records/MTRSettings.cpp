
#include "MTRSettings.h"

namespace XLS
{

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
	_UINT32 temp1, temp2;
#pragma message("############################ frtHeader skipped here")
	record >> temp1 >> temp2 >> cUserThreadCount;

	fMTREnabled			= temp1;
	fUserSetThreadCount	= temp2;
}

} // namespace XLS

