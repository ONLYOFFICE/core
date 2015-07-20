
#include "PtgAttrSemi.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrSemi::clone()
{
	return BiffStructurePtr(new PtgAttrSemi(*this));
}


void PtgAttrSemi::storeFields(CFRecord& record)
{
	record.reserveNunBytes(2); // unused
}


void PtgAttrSemi::loadFields(CFRecord& record)
{
	record.skipNunBytes(2); // unused
}


void PtgAttrSemi::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	// volatile property is not seen in the textual form
}


} // namespace XLS

