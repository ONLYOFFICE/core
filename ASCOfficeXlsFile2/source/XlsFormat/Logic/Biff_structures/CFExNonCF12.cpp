
#include "CFExNonCF12.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CFExNonCF12::clone()
{
	return BiffStructurePtr(new CFExNonCF12(*this));
}


void CFExNonCF12::store(CFRecord& record)
{
#pragma message("####################### CFExNonCF12 record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CFExNonCF12 record has no BiffStructure::store() implemented.");
	//record << something;
}


void CFExNonCF12::load(CFRecord& record)
{
	unsigned char flags;
	record >> icf >> cp >> icfTemplate >> ipriority_ >> flags;
	
	fActive		= GETBIT(flags, 0);
	fStopIfTrue = GETBIT(flags, 1);

	record >> fHasDXF;
	
	if(fHasDXF)
	{
		record >> dxf;
	}
	unsigned char cbTemplateParm;
	record >> cbTemplateParm;
	record >> rgbTemplateParms;
}


} // namespace XLS

