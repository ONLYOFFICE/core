#include "precompiled_xls.h"
#include "PtgAttrSpaceSemi.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrSpaceSemi::clone()
{
	return BiffStructurePtr(new PtgAttrSpaceSemi(*this));
}


void PtgAttrSpaceSemi::storeFields(CFRecord& record)
{
	record.reserveNunBytes(2); // I do not use it, but the data present
}


void PtgAttrSpaceSemi::loadFields(CFRecord& record)
{
	record.skipNunBytes(2); // I do not use it, but the data present
}


void PtgAttrSpaceSemi::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	// Display token that shall be skipped
}


} // namespace XLS

