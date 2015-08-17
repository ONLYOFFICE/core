
#include "FactoidData.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


FactoidData::FactoidData()
{
}


FactoidData::FactoidData(CFRecord& record)
{
	load(record);
}


BiffStructurePtr FactoidData::clone()
{
	return BiffStructurePtr(new FactoidData(*this));
}

void FactoidData::store(CFRecord& record)
{
#pragma message("####################### FactoidData record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FactoidData record has no BiffStructure::store() implemented.");
	//record << something;
}


void FactoidData::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fDelete = GETBIT(flags, 0);
	fXMLBased = GETBIT(flags, 0);
	record >> propertyBag;
}


} // namespace XLS

