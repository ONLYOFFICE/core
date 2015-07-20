
#include "StartObject.h"
#include <Logic/Biff_structures/FrtHeaderOld.h>

namespace XLS
{;

StartObject::StartObject()
{
}


StartObject::~StartObject()
{
}


BaseObjectPtr StartObject::clone()
{
	return BaseObjectPtr(new StartObject(*this));
}


void StartObject::writeFields(CFRecord& record)
{
	FrtHeaderOld rt(rt_StartObject);
	record << rt;

	iObjectContext = 0;
	iObjectInstance2 = 0;
	record << iObjectKind << iObjectContext << iObjectInstance1 << iObjectInstance2;
}


void StartObject::readFields(CFRecord& record)
{
	FrtHeaderOld rt;
	record >> rt;
	record >> iObjectKind >> iObjectContext >> iObjectInstance1 >> iObjectInstance2;
}

} // namespace XLS

