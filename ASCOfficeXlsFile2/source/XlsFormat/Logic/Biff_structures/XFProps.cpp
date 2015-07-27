
#include "XFProps.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFProps::clone()
{
	return BiffStructurePtr(new XFProps(*this));
}



void XFProps::store(CFRecord& record)
{
#pragma message("####################### XFProps record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFProps record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFProps::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> cprops;
	for(int i = 0; i < cprops; ++i)
	{
		XFProp prop;
		record >> prop;
		xfPropArray.push_back(prop);
	}
}


} // namespace XLS
