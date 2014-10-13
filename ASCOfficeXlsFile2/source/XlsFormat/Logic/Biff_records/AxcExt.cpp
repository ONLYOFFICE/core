#include "precompiled_xls.h"
#include "AxcExt.h"

namespace XLS
{;

AxcExt::AxcExt()
{
}


AxcExt::~AxcExt()
{
}


BaseObjectPtr AxcExt::clone()
{
	return BaseObjectPtr(new AxcExt(*this));
}


void AxcExt::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fAutoMin);
	SETBIT(flags, 1, fAutoMax);
	SETBIT(flags, 2, fAutoMajor);
	SETBIT(flags, 3, fAutoMinor);
	SETBIT(flags, 4, fDateAxis);
	SETBIT(flags, 5, fAutoBase);
	SETBIT(flags, 6, fAutoCross);
	SETBIT(flags, 7, fAutoDate);
	record >> catMin >> catMax >> catMajor >> duMajor >> catMinor >> duMinor >> duBase >> catCrossDate >> flags;

}


void AxcExt::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> catMin >> catMax >> catMajor >> duMajor >> catMinor >> duMinor >> duBase >> catCrossDate >> flags;

	fAutoMin = GETBIT(flags, 0);
	fAutoMax = GETBIT(flags, 1);
	fAutoMajor = GETBIT(flags, 2);
	fAutoMinor = GETBIT(flags, 3);
	fDateAxis = GETBIT(flags, 4);
	fAutoBase = GETBIT(flags, 5);
	fAutoCross = GETBIT(flags, 6);
	fAutoDate = GETBIT(flags, 7);
}

} // namespace XLS

