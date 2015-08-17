
#include "ExtPtgRef3D.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


ExtPtgRef3D::ExtPtgRef3D(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr ExtPtgRef3D::clone()
{
	return BiffStructurePtr(new ExtPtgRef3D(*this));
}

void ExtPtgRef3D::store(CFRecord& record)
{
	record << iTabs << loc;
}


void ExtPtgRef3D::load(CFRecord& record)
{
	record >> iTabs >> loc;
}


void ExtPtgRef3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
#pragma message("####################### ExtPtgRef3D record is not implemented")
	Log::info("ExtPtgRef3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

