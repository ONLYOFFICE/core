
#include "XFPropGradient.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XFPropGradient::clone()
{
	return BiffStructurePtr(new XFPropGradient(*this));
}

void XFPropGradient::store(CFRecord& record)
{
#pragma message("####################### XFPropGradient record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradient record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradient::load(CFRecord& record)
{
	_UINT32 temp;
	record >> temp >> numDegree >> numFillToLeft >> numFillToRight >> 
		numFillToTop >> numFillToBottom;
	
	type1 = temp;
}


} // namespace XLS
