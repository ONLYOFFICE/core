
#include "ExtPtgArea3D.h"
#include <Binary/CFRecord.h>
#include "CellRangeRef.h"

namespace XLS
{


ExtPtgArea3D::ExtPtgArea3D(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr ExtPtgArea3D::clone()
{
	return BiffStructurePtr(new ExtPtgArea3D(*this));
}


void ExtPtgArea3D::store(CFRecord& record)
{
	record << iTabs << area;
}


void ExtPtgArea3D::load(CFRecord& record)
{
	record >> iTabs >> area;
}


void ExtPtgArea3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
#pragma message("####################### ExtPtgArea3D record is not implemented")
	Log::info("ExtPtgArea3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

