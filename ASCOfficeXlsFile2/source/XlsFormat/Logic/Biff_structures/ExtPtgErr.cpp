
#include "ExtPtgErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ExtPtgErr::clone()
{
	return BiffStructurePtr(new ExtPtgErr(*this));
}


void ExtPtgErr::store(CFRecord& record)
{
	record << err;
}


void ExtPtgErr::load(CFRecord& record)
{
	record >> err;
}


void ExtPtgErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	ptg_stack.push(err.toString());
}


} // namespace XLS

