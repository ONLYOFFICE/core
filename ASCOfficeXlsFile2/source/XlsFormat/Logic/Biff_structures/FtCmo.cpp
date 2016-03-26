
#include "FtCmo.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FtCmo::clone()
{
	return BiffStructurePtr(new FtCmo(*this));
}


void FtCmo::store(CFRecord& record)
{
	unsigned short ft = 0x15; // reserved
	unsigned short cb = 0x12; // reserved
	record << ft << cb;

	unsigned short flags = 0;
	SETBIT(flags, 0, fLocked);
	SETBIT(flags, 2, fDefaultSize);
	SETBIT(flags, 3, fPublished);
	SETBIT(flags, 4, fPrint);
	SETBIT(flags, 7, fDisabled);
	SETBIT(flags, 8, fUIObj);
	SETBIT(flags, 9, fRecalcObj);
	SETBIT(flags, 12, fRecalcObjAlways);

	record << ot << id << flags;
	record.reserveNunBytes(2); // unused8
}


void FtCmo::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	record >> ot >> id;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char flags;
		record >> flags;
		
		fLocked			= GETBIT(flags, 0);
		fDefaultSize	= GETBIT(flags, 2);
		fPublished		= GETBIT(flags, 3);
		fPrint			= GETBIT(flags, 4);
		fDisabled		= GETBIT(flags, 7);
		
		fUIObj			= false;
		fRecalcObj		= false;
		fRecalcObjAlways = false;
	}
	else
	{
		unsigned short flags;
		record >> flags;
		record.skipNunBytes(12); // unused

		fLocked			= GETBIT(flags, 0);
		fDefaultSize	= GETBIT(flags, 2);
		fPublished		= GETBIT(flags, 3);
		fPrint			= GETBIT(flags, 4);
		fDisabled		= GETBIT(flags, 7);
		fUIObj			= GETBIT(flags, 8);
		fRecalcObj		= GETBIT(flags, 9);
		fRecalcObjAlways = GETBIT(flags, 12);
	}
}


} // namespace XLS

