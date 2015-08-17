
#include "CFExTemplateParams.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CFExTemplateParams::clone()
{
	return BiffStructurePtr(new CFExTemplateParams(*this));
}


void CFExTemplateParams::store(CFRecord& record)
{
#pragma message("####################### CFExTemplateParams record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CFExTemplateParams record has no BiffStructure::store() implemented.");
	//record << something;
}


void CFExTemplateParams::load(CFRecord& record)
{
	record.loadAnyData(data);
}

} // namespace XLS

