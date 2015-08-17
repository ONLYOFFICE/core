
#include "SDContainer.h"
#include <Binary/CFRecord.h>

namespace XLS
{


SDContainer::SDContainer()
{
}


SDContainer::SDContainer(CFRecord& record)
{
	load(record);
}


BiffStructurePtr SDContainer::clone()
{
	return BiffStructurePtr(new SDContainer(*this));
}


void SDContainer::store(CFRecord& record)
{
#pragma message("####################### SDContainer record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SDContainer record has no BiffStructure::store() implemented.");
	//record << something;
}


void SDContainer::load(CFRecord& record)
{
	unsigned int cbSD;
	record >> cbSD >> sd;
}


} // namespace XLS

