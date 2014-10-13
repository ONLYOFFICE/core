#include "precompiled_xls.h"
#include "AutoFilterInfo.h"

namespace XLS
{;

AutoFilterInfo::AutoFilterInfo()
{
}


AutoFilterInfo::~AutoFilterInfo()
{
}


BaseObjectPtr AutoFilterInfo::clone()
{
	return BaseObjectPtr(new AutoFilterInfo(*this));
}


void AutoFilterInfo::writeFields(CFRecord& record)
{
	record << cEntries;
}


void AutoFilterInfo::readFields(CFRecord& record)
{
	record >> cEntries;
}

} // namespace XLS

