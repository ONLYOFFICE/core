
#include "PropertyBagStore.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


XLS::BiffStructurePtr PropertyBagStore::clone()
{
	return XLS::BiffStructurePtr(new PropertyBagStore(*this));
}



void PropertyBagStore::store(XLS::CFRecord& record)
{
#pragma message("####################### PropertyBagStore record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PropertyBagStore record has no BiffStructure::store() implemented.");
	//record << something;
}


void PropertyBagStore::load(XLS::CFRecord& record)
{
	_UINT32 cFactoidType;
	record >> cFactoidType;
	
	for(size_t i = 0; i < cFactoidType; ++i)
	{
		FactoidType f;
		record >> f;
		factoidTypes.push_back(f);
	}
	record >> cbHdr >> sVer;
	record.skipNunBytes(4); // ignored
	_UINT32 cste;
	record >> cste;
	for(size_t i = 0; i < cste; ++i)
	{
		PBString f;
		record >> f;
		stringTable.push_back(f);
	}
}


} // namespace OSHARED

