
#include "PtgAttrSum.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrSum::clone()
{
	return BiffStructurePtr(new PtgAttrSum(*this));
}


void PtgAttrSum::storeFields(CFRecord& record)
{
	record.reserveNunBytes(2); // unused
}


void PtgAttrSum::loadFields(CFRecord& record)
{
	record.skipNunBytes(2); // unused
}


void PtgAttrSum::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	addFuncWrapper(ptg_stack, L"SUM");
}

} // namespace XLS

