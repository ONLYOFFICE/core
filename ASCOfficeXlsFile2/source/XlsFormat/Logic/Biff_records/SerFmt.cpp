
#include "SerFmt.h"

namespace XLS
{;

SerFmt::SerFmt()
{
}


SerFmt::~SerFmt()
{
}


BaseObjectPtr SerFmt::clone()
{
	return BaseObjectPtr(new SerFmt(*this));
}


void SerFmt::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fSmoothedLine);
	SETBIT(flags, 1, f3DBubbles);
	SETBIT(flags, 2, fArShadow);
	record << flags;
}


void SerFmt::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fSmoothedLine = GETBIT(flags, 0);
	f3DBubbles = GETBIT(flags, 1);
	fArShadow = GETBIT(flags, 2);
}

} // namespace XLS

