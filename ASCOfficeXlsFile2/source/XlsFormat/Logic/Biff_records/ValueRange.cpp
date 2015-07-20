
#include "ValueRange.h"

namespace XLS
{;

ValueRange::ValueRange()
{
}


ValueRange::~ValueRange()
{
}


BaseObjectPtr ValueRange::clone()
{
	return BaseObjectPtr(new ValueRange(*this));
}


void ValueRange::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoMin);
	SETBIT(flags, 1, fAutoMax);
	SETBIT(flags, 2, fAutoMajor);
	SETBIT(flags, 3, fAutoMinor);
	SETBIT(flags, 4, fAutoCross);
	SETBIT(flags, 5, fLog);
	SETBIT(flags, 6, fReversed);
	SETBIT(flags, 7, fMaxCross);
	record << numMin << numMax << numMajor << numMinor << numCross << flags;
}


void ValueRange::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> numMin >> numMax >> numMajor >> numMinor >> numCross >> flags;

	fAutoMin = GETBIT(flags, 0);
	fAutoMax = GETBIT(flags, 1);
	fAutoMajor = GETBIT(flags, 2);
	fAutoMinor = GETBIT(flags, 3);
	fAutoCross = GETBIT(flags, 4);
	fLog = GETBIT(flags, 5);
	fReversed = GETBIT(flags, 6);
	fMaxCross = GETBIT(flags, 7);
}

} // namespace XLS

