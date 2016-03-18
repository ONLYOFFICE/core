
#include "FtGboData.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FtGboData::clone()
{
	return BiffStructurePtr(new FtGboData(*this));
}



void FtGboData::store(CFRecord& record)
{
	unsigned short ft = 0x000F; // reserved
	unsigned short cb = 0x0006; // reserved
	record << ft << cb;

	record << accel;
	record.reserveNunBytes(2); // reserved
	unsigned short flags = 0;
	SETBIT(flags, 0, fNo3d);
	record << flags;
}


void FtGboData::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	record >> accel;
	record.skipNunBytes(2); // reserved
	unsigned short flags;
	record >> flags;
	fNo3d = GETBIT(flags, 0);
}


} // namespace XLS

