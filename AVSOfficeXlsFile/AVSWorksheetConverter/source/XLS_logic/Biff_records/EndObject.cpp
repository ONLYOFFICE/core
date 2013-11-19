#include "stdafx.h"
#include "EndObject.h"
#include <XLS_logic/Biff_structures/FrtHeaderOld.h>

namespace XLS
{;

EndObject::EndObject()
{
}


EndObject::~EndObject()
{
}


BaseObjectPtr EndObject::clone()
{
	return BaseObjectPtr(new EndObject(*this));
}


void EndObject::writeFields(CFRecord& record)
{
	FrtHeaderOld rt(rt_EndObject);
	record >> rt;
	WORD unused1 = 0, unused2 = 0, unused3 = 0;
	record << iObjectKind << unused1 << unused2 << unused3;
}


void EndObject::readFields(CFRecord& record)
{
	FrtHeaderOld rt;
	record >> rt;
	WORD unused1, unused2, unused3;
	record >> iObjectKind >> unused1 >> unused2 >> unused3;
}

} // namespace XLS

