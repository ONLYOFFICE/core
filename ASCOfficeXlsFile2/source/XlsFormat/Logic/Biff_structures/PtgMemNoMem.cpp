
#include "PtgMemNoMem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgMemNoMem::clone()
{
	return BiffStructurePtr(new PtgMemNoMem(*this));
}


void PtgMemNoMem::storeFields(CFRecord& record)
{
	record.reserveNunBytes(4); // unused
	record << cce;
}


void PtgMemNoMem::loadFields(CFRecord& record)
{
	record.skipNunBytes(4); // unused
	record >> cce;
}


void PtgMemNoMem::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
}



} // namespace XLS

