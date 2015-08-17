
#include "XFExtNoFRT.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


XFExtNoFRT::XFExtNoFRT()
{
}


XFExtNoFRT::XFExtNoFRT(CFRecord& record)
{
	load(record);
}


BiffStructurePtr XFExtNoFRT::clone()
{
	return BiffStructurePtr(new XFExtNoFRT(*this));
}


void XFExtNoFRT::store(CFRecord& record)
{
#pragma message("####################### XFExtNoFRT record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFExtNoFRT record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFExtNoFRT::load(CFRecord& record)
{
	record.skipNunBytes(6); // reserved
	unsigned short cexts;
	record >> cexts;
	for(unsigned short i = 0; i < cexts; ++i)
	{
		ExtProp prop;
		record >> prop;
		rgExt.push_back(prop);
	}
}


} // namespace XLS

