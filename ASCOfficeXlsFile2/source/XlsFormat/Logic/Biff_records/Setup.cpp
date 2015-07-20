
#include "Setup.h"

namespace XLS
{;

Setup::Setup()
// the following may appear uninitialized but we have to store them
:	iPaperSize(0), iScale(255), iRes(0), iVRes(0), iCopies(0), fNoOrient(false), fPortrait(false), iPageStart(1)
{
}


Setup::~Setup()
{
}


BaseObjectPtr Setup::clone()
{
	return BaseObjectPtr(new Setup(*this));
}


void Setup::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fLeftToRight);
	SETBIT(flags, 1, fPortrait);
	SETBIT(flags, 2, fNoPls);
	SETBIT(flags, 3, fNoColor);
	SETBIT(flags, 4, fDraft);
	SETBIT(flags, 5, fNotes);
	SETBIT(flags, 6, fNoOrient);
	SETBIT(flags, 7, fUsePage);
	SETBIT(flags, 9, fEndNotes);
	SETBITS(flags, 10, 11, iErrors);

	record << iPaperSize << iScale << iPageStart << iFitWidth << iFitHeight << flags;
	record << iRes << iVRes << numHdr << numFtr << iCopies;
}


void Setup::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> iPaperSize >> iScale >> iPageStart >> iFitWidth >> iFitHeight >> flags;
	fLeftToRight = GETBIT(flags, 0);
	fPortrait = GETBIT(flags, 1);
	fNoPls = GETBIT(flags, 2);
	fNoColor = GETBIT(flags, 3);
	fDraft = GETBIT(flags, 4);
	fNotes = GETBIT(flags, 5);
	fNoOrient = GETBIT(flags, 6);
	fUsePage = GETBIT(flags, 7);
	fEndNotes = GETBIT(flags, 9);
	iErrors = GETBITS(flags, 10, 11);

	record >> iRes >> iVRes >> numHdr >> numFtr >> iCopies;
}

} // namespace XLS

