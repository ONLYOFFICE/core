//#include "precompiled_xls.h"
#include "Area.h"

namespace XLS
{;

Area::Area()
{
}


Area::~Area()
{
}


BaseObjectPtr Area::clone()
{
	return BaseObjectPtr(new Area(*this));
}


void Area::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fStacked);
	SETBIT(flags, 1, f100);
	SETBIT(flags, 2, fHasShadow);
	record << flags;
}


void Area::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> flags;
	fStacked = GETBIT(flags, 0);
	f100 = GETBIT(flags, 1);
	fHasShadow = GETBIT(flags, 2);
}

} // namespace XLS

