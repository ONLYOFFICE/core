
#include "ExternDocName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ExternDocName::clone()
{
	return BiffStructurePtr(new ExternDocName(*this));
}



void ExternDocName::store(CFRecord& record)
{
#pragma message("####################### ExternDocName record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternDocName record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternDocName::load(CFRecord& record)
{
	record >> ixals;
	record.skipNunBytes(2); // reserved
	record >> extName;
	nameDefinition.load(record);
}


} // namespace XLS

