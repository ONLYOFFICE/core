
#include "AFDOperBoolErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr AFDOperBoolErr::clone()
{
	return BiffStructurePtr(new AFDOperBoolErr(*this));
}


void AFDOperBoolErr::store(CFRecord& record)
{	
}


void AFDOperBoolErr::load(CFRecord& record)
{
	record >> bes;
	
	record.skipNunBytes(2);
	record.skipNunBytes(4);
}


} // namespace XLS

