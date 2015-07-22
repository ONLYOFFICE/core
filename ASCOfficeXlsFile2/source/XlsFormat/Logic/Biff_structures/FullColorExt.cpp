
#include "FullColorExt.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FullColorExt::clone()
{
	return BiffStructurePtr(new FullColorExt(*this));
}

void FullColorExt::store(CFRecord& record)
{
#pragma message("####################### FullColorExt record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FullColorExt record has no BiffStructure::store() implemented.");
	//record << something;
}


void FullColorExt::load(CFRecord& record)
{
	record >> xclrType >> icv >> nTintShade >> xclrValue;
	record.skipNunBytes(8); //unused
}


} // namespace XLS
