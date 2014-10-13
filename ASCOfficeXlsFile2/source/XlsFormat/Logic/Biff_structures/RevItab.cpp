#include "precompiled_xls.h"
#include "RevItab.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


RevItab::RevItab()
{
}


RevItab::RevItab(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevItab::clone()
{
	return BiffStructurePtr(new RevItab(*this));
}


void RevItab::store(CFRecord& record)
{
#pragma message("####################### RevItab record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RevItab record has no BiffStructure::store() implemented.");
	//record << something;
}


void RevItab::load(CFRecord& record)
{
	record >> type;
	switch(type)
	{
		case 0x00: 
			record >> tabid;
			break;
		case 0x01: 
			record >> sheet;
			break;
	}
}


} // namespace XLS

