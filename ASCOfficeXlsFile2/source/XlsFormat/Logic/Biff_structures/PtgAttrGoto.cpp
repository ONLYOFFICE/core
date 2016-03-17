
#include "PtgAttrGoto.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgAttrGoto::clone()
{
	return BiffStructurePtr(new PtgAttrGoto(*this));
}

void PtgAttrGoto::storeFields(CFRecord& record)
{
	record << offset;
}


void PtgAttrGoto::loadFields(CFRecord& record)
{
	record >> offset;
}


void PtgAttrGoto::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
}


} // namespace XLS

