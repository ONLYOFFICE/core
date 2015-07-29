
#include "BookExt_Conditional11.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr BookExt_Conditional11::clone()
{
	return BiffStructurePtr(new BookExt_Conditional11(*this));
}

void BookExt_Conditional11::store(CFRecord& record)
{
	unsigned char flags = 0;
	SETBIT(flags, 0, fBuggedUserAboutSolution);
	SETBIT(flags, 1, fShowInkAnnotation);
	record << flags;
}


void BookExt_Conditional11::load(CFRecord& record)
{
	unsigned char flags;
	record >> flags;
	fBuggedUserAboutSolution = GETBIT(flags, 0);
	fShowInkAnnotation = GETBIT(flags, 1);
}


} // namespace XLS

