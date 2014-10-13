#include "precompiled_xls.h"
#include "BopPop.h"

namespace XLS
{;

BopPop::BopPop()
{
}


BopPop::~BopPop()
{
}


BaseObjectPtr BopPop::clone()
{
	return BaseObjectPtr(new BopPop(*this));
}


void BopPop::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fHasShadow);
	record << pst << fAutoSplit << split << iSplitPos << pcSplitPercent << pcPie2Size << pcGap << numSplitValue << flags;
}


void BopPop::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> pst >> fAutoSplit >> split >> iSplitPos >> pcSplitPercent >> pcPie2Size >> pcGap >> numSplitValue >> flags;
	fHasShadow = GETBIT(flags, 0);
}

} // namespace XLS

