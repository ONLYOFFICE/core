#include "precompiled_xls.h"
#include "ColInfo.h"

namespace XLS
{;

ColInfo::ColInfo()
{
}


ColInfo::~ColInfo()
{
}


BaseObjectPtr ColInfo::clone()
{
	return BaseObjectPtr(new ColInfo(*this));
}


void ColInfo::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fHidden);
	SETBIT(flags, 1, fUserSet);
	SETBIT(flags, 2, fBestFit);
	SETBIT(flags, 3, fPhonetic);
	SETBITS(flags, 8, 10, iOutLevel);
	SETBIT(flags, 12, fCollapsed);
	record << colFirst << colLast << coldx << ixfe << flags;
	record.reserveNunBytes(2); // unused
}


void ColInfo::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> colFirst >> colLast >> coldx >> ixfe >> flags;
	fHidden = GETBIT(flags, 0);
	fUserSet = GETBIT(flags, 1);
	fBestFit = GETBIT(flags, 2);
	fPhonetic = GETBIT(flags, 3);
	iOutLevel = GETBITS(flags, 8, 10);
	fCollapsed = GETBIT(flags, 12);
	record.skipNunBytes(2); // unused
}

} // namespace XLS

