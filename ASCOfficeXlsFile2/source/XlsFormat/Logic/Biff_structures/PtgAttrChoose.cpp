
#include "PtgAttrChoose.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgAttrChoose::clone()
{
	return BiffStructurePtr(new PtgAttrChoose(*this));
}

void PtgAttrChoose::storeFields(CFRecord& record)
{
#pragma message("####################### PtgAttrChoose record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgAttrChoose record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgAttrChoose::loadFields(CFRecord& record)
{
	record >> cOffset;
	for(int i = 0; i < cOffset + 1; ++i)
	{
		unsigned short val;
		record >> val;
		rgOffset.push_back(val);
	}
}


void PtgAttrChoose::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
}


} // namespace XLS

