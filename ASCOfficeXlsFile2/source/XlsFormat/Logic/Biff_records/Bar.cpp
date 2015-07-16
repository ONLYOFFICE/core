#include "precompiled_xls.h"
#include "Bar.h"

namespace XLS
{;

Bar::Bar()
{
}


Bar::~Bar()
{
}


BaseObjectPtr Bar::clone()
{
	return BaseObjectPtr(new Bar(*this));
}


void Bar::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fTranspose);
	SETBIT(flags, 1, fStacked);
	SETBIT(flags, 2, f100);
	SETBIT(flags, 3, fHasShadow);
	record >> pcOverlap >> pcGap >> flags;
}


void Bar::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pcOverlap >> pcGap >> flags;
	fTranspose = GETBIT(flags, 0);
	fStacked = GETBIT(flags, 1);
	f100 = GETBIT(flags, 2);
	fHasShadow = GETBIT(flags, 3);
}

} // namespace XLS

