
#include "ExternOleDdeLink.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExternOleDdeLink::clone()
{
	return BiffStructurePtr(new ExternOleDdeLink(*this));
}


void ExternOleDdeLink::store(CFRecord& record)
{
#pragma message("####################### ExternOleDdeLink record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternOleDdeLink record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternOleDdeLink::load(CFRecord& record)
{
	record >> lStgName >> linkName;
	if(!record.isEOF())
	{
		record >> moper;
	}
}


} // namespace XLS

