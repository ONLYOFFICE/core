#include "stdafx.h"
#include "ShtProps.h"

namespace XLS
{;

ShtProps::ShtProps()
{
}


ShtProps::~ShtProps()
{
}


BaseObjectPtr ShtProps::clone()
{
	return BaseObjectPtr(new ShtProps(*this));
}


void ShtProps::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fManSerAlloc);
	SETBIT(flags, 1, fPlotVisOnly);
	SETBIT(flags, 2, fNotSizeWith);
	BIFF_BOOL fManPlotArea = fAlwaysAutoPlotArea;
	SETBIT(flags, 3, fManPlotArea);
	SETBIT(flags, 4, fAlwaysAutoPlotArea);
	record << flags << mdBlank;
	record.reserveNBytes(1); // reserved2
}


void ShtProps::readFields(CFRecord& record)
{
	WORD flags;
	record >> flags >> mdBlank;
	fManSerAlloc = GETBIT(flags, 0);
	fPlotVisOnly = GETBIT(flags, 1);
	fNotSizeWith = GETBIT(flags, 2);
	fAlwaysAutoPlotArea = GETBIT(flags, 4);
	record.skipNBytes(1); // reserved2
}

} // namespace XLS

