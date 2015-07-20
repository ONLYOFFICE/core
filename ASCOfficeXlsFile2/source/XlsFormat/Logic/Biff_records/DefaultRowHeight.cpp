
#include "DefaultRowHeight.h"

namespace XLS
{;

DefaultRowHeight::DefaultRowHeight()
{
}


DefaultRowHeight::~DefaultRowHeight()
{
}


BaseObjectPtr DefaultRowHeight::clone()
{
	return BaseObjectPtr(new DefaultRowHeight(*this));
}


void DefaultRowHeight::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fUnsynced);
	SETBIT(flags, 1, fDyZero);
	SETBIT(flags, 2, fExAsc);
	SETBIT(flags, 3, fExDsc);
	record << flags;
	if(!fDyZero)
	{
		record << miyRw;
	}
	else
	{
		record << miyRwHidden;
	}
}


void DefaultRowHeight::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fUnsynced = GETBIT(flags, 0);
	fDyZero = GETBIT(flags, 1);
	fExAsc = GETBIT(flags, 2);
	fExDsc = GETBIT(flags, 3);
	if(!fDyZero)
	{
		record >> miyRw;
	}
	else
	{
		record >> miyRwHidden;
	}
}

} // namespace XLS

