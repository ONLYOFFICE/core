
#include "NoteSh.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


NoteSh::NoteSh()
{
}


NoteSh::NoteSh(CFRecord& record)
{
	load(record);
}


BiffStructurePtr NoteSh::clone()
{
	return BiffStructurePtr(new NoteSh(*this));
}


void NoteSh::store(CFRecord& record)
{
#pragma message("####################### NoteSh record has no BiffStructure::store() implemented")
	Log::error(" Error!!! NoteSh record has no BiffStructure::store() implemented.");
	//record << something;
}


void NoteSh::load(CFRecord& record)
{
	unsigned short flags;
	record >> row >> col >> flags;
	fShow = GETBIT(flags, 1);
	fRwHidden = GETBIT(flags, 7);
	fColHidden = GETBIT(flags, 8);

	record >> idObj >> stAuthor;
	record.skipNunBytes(1); // unused
}


} // namespace XLS

