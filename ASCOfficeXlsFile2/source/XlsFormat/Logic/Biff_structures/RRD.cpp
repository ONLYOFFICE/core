
#include "RRD.h"
#include <Binary/CFRecord.h>

namespace XLS
{


RRD::RRD()
{
}


RRD::RRD(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RRD::clone()
{
	return BiffStructurePtr(new RRD(*this));
}



void RRD::store(CFRecord& record)
{
#pragma message("####################### RRD record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RRD record has no BiffStructure::store() implemented.");
	//record << something;
}


void RRD::load(CFRecord& record)
{
	unsigned short flags;
	record >> cbMemory >> revid >> revt >> flags;
	fAccepted = GETBIT(flags, 0);
	fUndoAction = GETBIT(flags, 1);
	fDelAtEdgeOfSort = GETBIT(flags, 3);
	record >> tabid;
}


} // namespace XLS

