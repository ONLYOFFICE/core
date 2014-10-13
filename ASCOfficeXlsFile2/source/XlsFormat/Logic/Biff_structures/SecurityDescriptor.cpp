#include "precompiled_xls.h"
#include "SecurityDescriptor.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


SecurityDescriptor::SecurityDescriptor()
{
}


SecurityDescriptor::SecurityDescriptor(CFRecord& record)
{
	load(record);
}


BiffStructurePtr SecurityDescriptor::clone()
{
	return BiffStructurePtr(new SecurityDescriptor(*this));
}


void SecurityDescriptor::store(CFRecord& record)
{
#pragma message("####################### SecurityDescriptor record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SecurityDescriptor record has no BiffStructure::store() implemented.");
	//record << something;
}


void SecurityDescriptor::load(CFRecord& record)
{
	record.loadAnyData(sd);
}


} // namespace XLS

