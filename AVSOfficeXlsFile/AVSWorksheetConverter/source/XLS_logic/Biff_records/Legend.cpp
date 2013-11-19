#include "stdafx.h"
#include "Legend.h"

namespace XLS
{;

Legend::Legend()
{
}


Legend::~Legend()
{
}


BaseObjectPtr Legend::clone()
{
	return BaseObjectPtr(new Legend(*this));
}


void Legend::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fAutoPosition);
	SETBIT(flags, 2, fAutoPosX);
	SETBIT(flags, 3, fAutoPosY);
	SETBIT(flags, 4, fVert);
	SETBIT(flags, 5, fWasDataTable);
	record << x << y << dx << dy;
	record.reserveNBytes(1); // unused
	record << wSpace << flags;
}


void Legend::readFields(CFRecord& record)
{
	WORD flags;
	record >> x >> y >> dx >> dy;
	record.skipNBytes(1); // unused
	record >> wSpace >> flags;

	fAutoPosition = GETBIT(flags, 0);
	fAutoPosX = GETBIT(flags, 2);
	fAutoPosY = GETBIT(flags, 3);
	fVert = GETBIT(flags, 4);
	fWasDataTable = GETBIT(flags, 5);	
}

} // namespace XLS

