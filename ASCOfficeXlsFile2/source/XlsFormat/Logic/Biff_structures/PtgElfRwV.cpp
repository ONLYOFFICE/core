
#include "PtgElfRwV.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgElfRwV::clone()
{
	return BiffStructurePtr(new PtgElfRwV(*this));
}


void PtgElfRwV::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgElfRwV::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgElfRwV::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	ptg_stack.push(CellRangeRef(loc, CellRangeRef::stRow).toString());
}



} // namespace XLS

