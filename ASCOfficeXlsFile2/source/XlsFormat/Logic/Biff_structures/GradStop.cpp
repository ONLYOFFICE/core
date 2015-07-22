
#include "GradStop.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr GradStop::clone()
{
	return BiffStructurePtr(new GradStop(*this));
}


void GradStop::store(CFRecord& record)
{
#pragma message("####################### GradStop record has no BiffStructure::store() implemented")
	Log::error(" Error!!! GradStop record has no BiffStructure::store() implemented.");
	//record << something;
}


void GradStop::load(CFRecord& record)
{
	record >> xclrType;
	if(2 == xclrType)
	{
		record >> xclrValue_rgb;
	}
	else
	{
		record >> xclrValue;
	}

	record >> numPosition >> numTint;
}


} // namespace XLS
