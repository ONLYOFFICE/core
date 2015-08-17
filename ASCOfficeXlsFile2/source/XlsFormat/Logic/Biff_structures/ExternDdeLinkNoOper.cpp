
#include "ExternDdeLinkNoOper.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExternDdeLinkNoOper::clone()
{
	return BiffStructurePtr(new ExternDdeLinkNoOper(*this));
}


void ExternDdeLinkNoOper::store(CFRecord& record)
{
#pragma message("####################### ExternDdeLinkNoOper record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternDdeLinkNoOper record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternDdeLinkNoOper::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved
	record >> linkName;
}


} // namespace XLS

