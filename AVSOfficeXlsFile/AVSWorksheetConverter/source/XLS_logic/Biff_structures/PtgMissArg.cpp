#include "stdafx.h"
#include "PtgMissArg.h"
#include <XLS_bin/CFRecord.h>

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


void PtgMissArg::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push(L"");
}



} // namespace XLS

