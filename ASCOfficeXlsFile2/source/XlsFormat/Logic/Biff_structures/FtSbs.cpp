
#include "FtSbs.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FtSbs::clone()
{
	return BiffStructurePtr(new FtSbs(*this));
}


void FtSbs::store(CFRecord& record)
{
	unsigned short ft = 0x0C; // reserved
	unsigned short cb = 0x14; // reserved
	record << ft << cb;

	record.reserveNunBytes(4); // unused1

	unsigned short flags = 0;
	SETBIT(flags, 0, fDraw);
	SETBIT(flags, 1, fDrawSliderOnly);
	SETBIT(flags, 2, fTrackElevator);
	SETBIT(flags, 3, fNo3d);

	record << iVal << iMin << iMax << dInc << dPage << fHoriz << dxScroll << flags;
}


void FtSbs::load(CFRecord& record)
{
	//record.skipNunBytes(4); // reserved

	record >> ft;
	record >> cb;

	record.skipNunBytes(4); // unused1

	unsigned short flags;

	record >> iVal >> iMin >> iMax >> dInc >> dPage >> fHoriz >> dxScroll >> flags;

	fDraw = GETBIT(flags, 0);
	fDrawSliderOnly = GETBIT(flags, 1);
	fTrackElevator = GETBIT(flags, 2);
	fNo3d = GETBIT(flags, 3);
}


} // namespace XLS

