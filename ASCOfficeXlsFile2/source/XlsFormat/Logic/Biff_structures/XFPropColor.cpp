
#include "XFPropColor.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropColor::clone()
{
	return BiffStructurePtr(new XFPropColor(*this));
}


void XFPropColor::store(CFRecord& record)
{
#pragma message("####################### XFPropColor record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropColor record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropColor::load(CFRecord& record)
{
	unsigned char flags;
	record >> flags;
	fValidRGBA = GETBIT(flags, 0);
	xclrType = GETBITS(flags, 1, 7);
	record >> icv >> nTintShade >> dwRgba;
}


} // namespace XLS
