
#include "PtgMemErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgMemErr::clone()
{
	return BiffStructurePtr(new PtgMemErr(*this));
}


void PtgMemErr::storeFields(CFRecord& record)
{
	record << err;
	record.reserveNunBytes(3); // unused
	record << cce;
}


void PtgMemErr::loadFields(CFRecord& record)
{
	record >> err;
	record.skipNunBytes(3); // unused
	record >> cce;
}


void PtgMemErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	//ptg_stack.push("#CACHE(" + STR::int2str(getOffsetInRecord() + getSizeOfStruct() + cce, 10) + ")!" + err.toString());
}


} // namespace XLS

