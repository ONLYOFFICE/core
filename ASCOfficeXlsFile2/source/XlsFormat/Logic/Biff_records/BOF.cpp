
#include "BOF.h"
#include "../../Binary/CFStream.h"


namespace XLS
{;

BOF::BOF()
{
}


BOF::~BOF()
{
}


BaseObjectPtr BOF::clone()
{
	return BaseObjectPtr(new BOF(*this));
}


void BOF::writeFields(CFRecord& record)
{
	if(BOF::st_Worksheet == dt)
	{
		record.registerDelayedFilePointerSource(rt_BoundSheet8);
	}
	record << vers << dt << rupBuild << rupYear;
	unsigned int flags = 0;
	SETBIT(flags, 0, fWin);
	SETBIT(flags, 1, fRisc);
	SETBIT(flags, 2, fBeta);
	SETBIT(flags, 3, fWinAny);
	SETBIT(flags, 4, fMacAny);
	SETBIT(flags, 5, fBetaAny);
	SETBIT(flags, 8, fRiscAny);
	SETBIT(flags, 9, fOOM);
	SETBIT(flags, 10, fGlJmp);
	SETBIT(flags, 13, fFontLimit);
	SETBITS(flags, 14,17, verXLHigh);
	record << flags;
	record << verLowestBiff;
	unsigned char flags2 = 0;
	SETBITS(flags2, 0, 3, verLastXLSaved);
	record << flags2;
	record.reserveNunBytes(2); // reserved
}


void BOF::readFields(CFRecord& record)
{
	record >> vers >> dt >> rupBuild >> rupYear;
	
	if (vers == (_UINT16)0x600)
	{
		unsigned int flags;
		record >> flags;
		fWin = GETBIT(flags, 0);
		fRisc = GETBIT(flags, 1);
		fBeta = GETBIT(flags, 2);
		fWinAny = GETBIT(flags, 3);
		fMacAny = GETBIT(flags, 4);
		fBetaAny = GETBIT(flags, 5);
		fRiscAny = GETBIT(flags, 8);
		fOOM = GETBIT(flags, 9);
		fGlJmp = GETBIT(flags, 10);
		fFontLimit = GETBIT(flags, 13);
		verXLHigh = static_cast<unsigned char>(GETBITS(flags, 14, 17));

		record >> verLowestBiff;
		unsigned char flags2;
		record >> flags2;
		verLastXLSaved = GETBITS(flags2, 0, 3);
		stream_ptr = record.getStreamPointer();
		record.skipNunBytes(2); // reserved
	}
	else if (vers == (_UINT16)0x500)//testdoc01.xls
	{
	}
}

unsigned short BOF::getSubstreamType()
{
	return dt;
}


} // namespace XLS


