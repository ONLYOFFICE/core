
#include "PtgAttrIf.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgAttrIf::clone()
{
	return BiffStructurePtr(new PtgAttrIf(*this));
}


void PtgAttrIf::storeFields(CFRecord& record)
{
	record << offset;
}


void PtgAttrIf::loadFields(CFRecord& record)
{
	record >> offset;
}


void PtgAttrIf::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
}


} // namespace XLS

