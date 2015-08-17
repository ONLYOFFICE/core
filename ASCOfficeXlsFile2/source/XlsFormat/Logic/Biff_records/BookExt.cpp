
#include "BookExt.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

BookExt::BookExt()
: cb(22)
{
}


BookExt::~BookExt()
{
}


BaseObjectPtr BookExt::clone()
{
	return BaseObjectPtr(new BookExt(*this));
}


void BookExt::writeFields(CFRecord& record)
{
	FrtHeader header(rt_BookExt);
	record << header;
	record << cb;
	unsigned int flags = 0;
	SETBIT(flags, 0, fDontAutoRecover);
	SETBIT(flags, 1, fHidePivotList);
	SETBIT(flags, 2, fFilterPrivacy);
	SETBIT(flags, 3, fEmbedFactoids);
	SETBITS(flags, 4, 5, mdFactoidDisplay);
	SETBIT(flags, 6, fSavedDuringRecovery);
	SETBIT(flags, 7, fCreatedViaMinimalSave);
	SETBIT(flags, 8, fOpenedViaDataRecovery);
	SETBIT(flags, 9, fOpenedViaSafeLoad);
	record << flags;

	record << grbit1;
	record << grbit2;

}


void BookExt::readFields(CFRecord& record)
{
	FrtHeader header(rt_BookExt);
	record >> header;

	record >> cb;
	unsigned int flags;
	record.loadAnyData(flags);
	fDontAutoRecover = GETBIT(flags, 0);
	fHidePivotList = GETBIT(flags, 1);
	fFilterPrivacy = GETBIT(flags, 2);
	fEmbedFactoids = GETBIT(flags, 3);
	mdFactoidDisplay = static_cast<unsigned char>(GETBITS(flags, 4, 5));
	fSavedDuringRecovery = GETBIT(flags, 6);
	fCreatedViaMinimalSave = GETBIT(flags, 7);
	fOpenedViaDataRecovery = GETBIT(flags, 8);
	fOpenedViaSafeLoad = GETBIT(flags, 9);



	if(cb > 20)
	{
		record >> grbit1;
	}
	if(cb > 21)
	{
		record >> grbit2;
	}

}

} // namespace XLS

