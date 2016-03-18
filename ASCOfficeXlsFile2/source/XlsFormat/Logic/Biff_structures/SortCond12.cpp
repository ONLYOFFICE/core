
#include "SortCond12.h"
#include <Binary/CFRecord.h>
#include "CellRangeRef.h"

namespace XLS
{


BiffStructurePtr SortCond12::clone()
{
	return BiffStructurePtr(new SortCond12(*this));
}

void SortCond12::store(CFRecord& record)
{
#pragma message("####################### SortCond12 record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SortCond12 record has no BiffStructure::store() implemented.");
	//record << something;
}


void SortCond12::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fSortDes	= GETBIT(flags, 0);
	sortOn		= GETBITS(flags, 1, 4);

	record >> rfx;

	switch(sortOn)
	{
		case 0x00:
		case 0x01:
		case 0x02:
			record >> condDataValue;
			break;
		case 0x03:
			record >> cfflag;
			break;
	}

	record >> cchSt;
	if(cchSt)
	{
		stSslist.setSize(cchSt);
		record >> stSslist;
	}
}


} // namespace XLS

