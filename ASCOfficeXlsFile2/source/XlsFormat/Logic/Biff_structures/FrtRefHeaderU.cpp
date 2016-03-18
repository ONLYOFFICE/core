
#include "FrtRefHeaderU.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{


BiffStructurePtr FrtRefHeaderU::clone()
{
	return BiffStructurePtr(new FrtRefHeaderU(*this));
}


void FrtRefHeaderU::store(CFRecord& record)
{
	record << rt << grbitFrt;
	record << ref8;
}


void FrtRefHeaderU::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
	record >> ref8;
}


} // namespace XLS

