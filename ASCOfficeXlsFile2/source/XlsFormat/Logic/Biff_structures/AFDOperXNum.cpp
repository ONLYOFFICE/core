
#include "AFDOperXNum.h"
#include <Binary/CFRecord.h>

namespace XLS
{

BiffStructurePtr AFDOperXNum::clone()
{
	return BiffStructurePtr(new AFDOperXNum(*this));
}

void AFDOperXNum::store(CFRecord& record)
{	
}


void AFDOperXNum::load(CFRecord& record)
{
	_UINT32 v1, v2;
	record >> v1 >> v2;

	val = 0;
}


} // namespace XLS

