
#include "PtgAttrSpace.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgAttrSpace::clone()
{
	return BiffStructurePtr(new PtgAttrSpace(*this));
}


void PtgAttrSpace::storeFields(CFRecord& record)
{
	record.reserveNunBytes(2); // I do not use it, but the data present
}


void PtgAttrSpace::loadFields(CFRecord& record)
{
	record.skipNunBytes(2); // I do not use it, but the data present
}


void PtgAttrSpace::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	// Display token, that shall be skipped
}


} // namespace XLS

