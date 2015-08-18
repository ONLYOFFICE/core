
#include "SharedProperty.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{

SharedProperty::SharedProperty()
{
}


SharedProperty::SharedProperty(XLS::CFRecord& record)
{
    load(record);
}

XLS::BiffStructurePtr SharedProperty::clone()
{
    return XLS::BiffStructurePtr(new SharedProperty(*this));
}


void SharedProperty::store(XLS::CFRecord& record)
{
#pragma message("####################### Property record has no BiffStructure::store() implemented")
	Log::error(" Error!!! Property record has no BiffStructure::store() implemented.");
	//record << something;
}


void SharedProperty::load(XLS::CFRecord& record)
{
	record >> keyIndex >> valueIndex;
}


} // namespace OSHARED

