
#include "List12BlockLevel.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr List12BlockLevel::clone()
{
	return BiffStructurePtr(new List12BlockLevel(*this));
}

List12BlockLevel::List12BlockLevel()
{		
}


List12BlockLevel::~List12BlockLevel()
{
}

void List12BlockLevel::store(CFRecord& record)
{	
}


void List12BlockLevel::load(CFRecord& record)
{	
	record >> cbdxfHeader;
	record >> istnHeader;
	record >> cbdxfData;
	record >> istnData;
	record >> cbdxfAgg;
	record >> istnAgg;
	record >> cbdxfBorder;
	record >> cbdxfHeaderBorder;
	record >> cbdxfAggBorder;
	
	if (cbdxfHeader)
		record >> dxfHeader;
	if (cbdxfData)
		record >> dxfData;
	if (cbdxfAgg)
		record >> dxfAgg;
	if (cbdxfBorder)
		record >> dxfBorder;
	if (cbdxfHeaderBorder)
		record >> dxfHeaderBorder;
	if (cbdxfAggBorder)
		record >> dxfAggBorder;
	if (istnHeader != -1)
		record >> stHeader;
	if (istnData != -1)
		record >> stData;
	if (istnAgg != -1)
		record >> stAgg;
}


} // namespace XLS

