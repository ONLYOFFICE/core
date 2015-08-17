
#include "XFPropGradientStop.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XFPropGradientStop::clone()
{
	return BiffStructurePtr(new XFPropGradientStop(*this));
}


void XFPropGradientStop::store(CFRecord& record)
{
#pragma message("####################### XFPropGradientStop record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradientStop record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradientStop::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused
	record >> numPosition >> color;
}


} // namespace XLS
