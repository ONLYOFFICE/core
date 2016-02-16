
#include "AFDOperRk.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr AFDOperRk::clone()
{
	return BiffStructurePtr(new AFDOperRk(*this));
}

void AFDOperRk::store(CFRecord& record)
{	
}


void AFDOperRk::load(CFRecord& record)
{
	// rk (4 unsigned chars): An RkNumber that specifies a numeric value.
	// unused1 (4 unsigned chars): Undefined and MUST be ignored.

	record >> rk;
	record.skipNunBytes(4);
}


} // namespace XLS

