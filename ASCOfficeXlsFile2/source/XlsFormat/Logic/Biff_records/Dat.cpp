
#include "Dat.h"

namespace XLS
{;

Dat::Dat()
{
}


Dat::~Dat()
{
}


BaseObjectPtr Dat::clone()
{
	return BaseObjectPtr(new Dat(*this));
}


void Dat::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fHasBordHorz);
	SETBIT(flags, 1, fHasBordVert);
	SETBIT(flags, 2, fHasBordOutline);
	SETBIT(flags, 3, fShowSeriesKey);
	record << flags;
}


void Dat::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fHasBordHorz = GETBIT(flags, 0);
	fHasBordVert = GETBIT(flags, 1);
	fHasBordOutline = GETBIT(flags, 2);
	fShowSeriesKey = GETBIT(flags, 3);
}

} // namespace XLS

