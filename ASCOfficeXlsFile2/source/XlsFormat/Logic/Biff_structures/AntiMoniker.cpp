
#include "AntiMoniker.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


XLS::BiffStructurePtr AntiMoniker::clone()
{
	return XLS::BiffStructurePtr(new AntiMoniker(*this));
}



void AntiMoniker::store(XLS::CFRecord& record)
{
#pragma message("####################### AntiMoniker record has no BiffStructure::store() implemented")
	Log::error(" Error!!! AntiMoniker record has no BiffStructure::store() implemented.");
	//record << something;
}


void AntiMoniker::load(XLS::CFRecord& record)
{
	record >> count;
}


} // namespace OSHARED

