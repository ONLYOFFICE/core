
#include "FeatProtection.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


FeatProtection::FeatProtection()
:	sdContainer_set(false)
{
}


FeatProtection::FeatProtection(CFRecord& record)
:	sdContainer_set(false)
{
	load(record);
}


BiffStructurePtr FeatProtection::clone()
{
	return BiffStructurePtr(new FeatProtection(*this));
}


void FeatProtection::store(CFRecord& record)
{
#pragma message("####################### FeatProtection record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FeatProtection record has no BiffStructure::store() implemented.");
	//record << something;
}


void FeatProtection::load(CFRecord& record)
{
	unsigned int flags;
	record >> flags;
	fSD = GETBIT(flags, 0);
	record >> wPassword >> stTitle;
	if(!record.isEOF())
	{
		record >> sdContainer;
		sdContainer_set = true;
	}
}


} // namespace XLS

