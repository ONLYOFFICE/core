
#include "FtPioGrbit.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FtPioGrbit::clone()
{
	return BiffStructurePtr(new FtPioGrbit(*this));
}


void FtPioGrbit::store(CFRecord& record)
{
	unsigned short ft = 0x08; // reserved
	unsigned short cb = 0x02; // reserved
	record << ft << cb;

	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoPict);
	SETBIT(flags, 1, fDde);
	SETBIT(flags, 2, fPrintCalc);
	SETBIT(flags, 3, fIcon);
	SETBIT(flags, 4, fCtl);
	SETBIT(flags, 5, fPrstm);
	SETBIT(flags, 7, fCamera);
	SETBIT(flags, 8, fDefaultSize);
	SETBIT(flags, 9, fAutoLoad);

	record << flags;
}


void FtPioGrbit::load(CFRecord& record)
{
	//record.skipNunBytes(4); // reserved
	unsigned short ft, cb;
	record >> ft;
	record >> cb;

	if (record.getDataSize() == record.getRdPtr())
	{
		record.RollRdPtrBack(4);
		return;
	}

	unsigned short flags;
	record >> flags;
	fAutoPict = GETBIT(flags, 0);
	fDde = GETBIT(flags, 1);
	fPrintCalc = GETBIT(flags, 2);
	fIcon = GETBIT(flags, 3);
	fCtl = GETBIT(flags, 4);
	fPrstm = GETBIT(flags, 5);
	fCamera = GETBIT(flags, 7);
	fDefaultSize = GETBIT(flags, 8);
	fAutoLoad = GETBIT(flags, 9);
}


} // namespace XLS

