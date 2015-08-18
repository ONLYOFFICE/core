
#include "PropertyBag.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{

PropertyBag::PropertyBag()
{
}

XLS::BiffStructurePtr PropertyBag::clone()
{
	return XLS::BiffStructurePtr(new PropertyBag(*this));
}


void PropertyBag::store(XLS::CFRecord& record)
{
#pragma message("####################### PropertyBag record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PropertyBag record has no BiffStructure::store() implemented.");
	//record << something;
}


void PropertyBag::load(XLS::CFRecord& record)
{
	unsigned short cProp;
	record >> id >> cProp;
	record.skipNunBytes(2); // ignored
	for (int i = 0; i < cProp ; i++)
	{
        OSHARED::SharedProperty prop;
        record >> prop;
		properties.push_back(prop);
	}
}


} // namespace OSHARED

