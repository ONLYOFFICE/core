
#include "PtgMissArg.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgMissArg::PtgMissArg()
:	OperandPtg(fixed_id)
{
}


BiffStructurePtr PtgMissArg::clone()
{
	return BiffStructurePtr(new PtgMissArg(*this));
}


void PtgMissArg::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(L"");
}



} // namespace XLS

