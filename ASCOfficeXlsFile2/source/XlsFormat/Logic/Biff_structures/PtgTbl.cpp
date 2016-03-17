
#include "PtgTbl.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgTbl::PtgTbl()
:	Ptg(fixed_id)
{
}


BiffStructurePtr PtgTbl::clone()
{
	return BiffStructurePtr(new PtgTbl(*this));
}

void PtgTbl::storeFields(CFRecord& record)
{
	record << row << col;
}


void PtgTbl::loadFields(CFRecord& record)
{
	record >> row >> col;
}


void PtgTbl::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	// The reference coordinates shall be obtained from row/column values.
	// No textual form need but the empty line.
	ptg_stack.push(L""); 
}


} // namespace XLS

