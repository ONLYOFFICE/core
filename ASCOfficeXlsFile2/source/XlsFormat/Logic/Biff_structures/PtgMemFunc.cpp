
#include "PtgMemFunc.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgMemFunc::clone()
{
	return BiffStructurePtr(new PtgMemFunc(*this));
}

void PtgMemFunc::storeFields(CFRecord& record)
{
	record << cce;
}


void PtgMemFunc::loadFields(CFRecord& record)
{
	record >> cce;
}


void PtgMemFunc::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	// no textual form
}


} // namespace XLS

