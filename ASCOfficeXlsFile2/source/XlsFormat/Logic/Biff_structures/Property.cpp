
#include "Property.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


XLS::BiffStructurePtr Property::clone()
{
	return XLS::BiffStructurePtr(new Property(*this));
}


void Property::store(XLS::CFRecord& record)
{
#pragma message("####################### Property record has no BiffStructure::store() implemented")
	Log::error(" Error!!! Property record has no BiffStructure::store() implemented.");
	//record << something;
}


void Property::load(XLS::CFRecord& record)
{
	record >> keyIndex >> valueIndex;
}


} // namespace OSHARED

