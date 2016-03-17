
#include "PtgSxName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgSxName::clone()
{
	return BiffStructurePtr(new PtgSxName(*this));
}


void PtgSxName::storeFields(CFRecord& record)
{
	record << sxIndex;
}


void PtgSxName::loadFields(CFRecord& record)
{
	record >> sxIndex;
}


void PtgSxName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
#pragma message("####################### PtgSxName struct is not implemented")
	Log::info("PtgSxName structure is not implemented.");
	ptg_stack.push(L"#REF!");
}


} // namespace XLS

