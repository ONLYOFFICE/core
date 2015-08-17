
#include "PtgMemNoMemN.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgMemNoMemN::clone()
{
	return BiffStructurePtr(new PtgMemNoMemN(*this));
}



void PtgMemNoMemN::store(CFRecord& record)
{
#pragma message("####################### PtgMemNoMemN record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgMemNoMemN record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgMemNoMemN::load(CFRecord& record)
{
	unsigned char rec_type;
	record >> rec_type;
	type1 = GETBITS(rec_type, 5, 6);
	record >> cce;
}


void PtgMemNoMemN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	// I assume it doesn't generate an error.
}




} // namespace XLS

