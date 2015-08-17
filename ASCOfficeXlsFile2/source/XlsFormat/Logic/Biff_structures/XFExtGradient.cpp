
#include "XFExtGradient.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XFExtGradient::clone()
{
	return BiffStructurePtr(new XFExtGradient(*this));
}


void XFExtGradient::store(CFRecord& record)
{
#pragma message("####################### XFExtGradient record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFExtGradient record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFExtGradient::load(CFRecord& record)
{
	record >> gradient >> cGradStops;
	for(size_t i = 0; i < cGradStops; ++i)
	{
		GradStop grad_stop;
		record >> grad_stop;
		rgGradStops.push_back(grad_stop);
	}
}


} // namespace XLS
