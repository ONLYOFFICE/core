
#include "FrtRefHeaderNoGrbit.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/CellRangeRef.h>
//#include <Exception/WrongAPIUsage.h>

namespace XLS
{


FrtRefHeaderNoGrbit::FrtRefHeaderNoGrbit(const CFRecordType::TypeId type)
:	rt(type)
{
}


FrtRefHeaderNoGrbit::FrtRefHeaderNoGrbit(const CFRecordType::TypeId type, const Ref8U& ref8_init)
:	rt(type),
	ref8(ref8_init)
{
}


BiffStructurePtr FrtRefHeaderNoGrbit::clone()
{
	return BiffStructurePtr(new FrtRefHeaderNoGrbit(*this));
}




void FrtRefHeaderNoGrbit::store(CFRecord& record)
{
	record << rt;
	record << ref8;
}


void FrtRefHeaderNoGrbit::load(CFRecord& record)
{
	record >> rt;
	record >> ref8;
}


} // namespace XLS

