#include "precompiled_xls.h"
#include "MarkerFormat.h"

namespace XLS
{;

MarkerFormat::MarkerFormat()
{
}


MarkerFormat::~MarkerFormat()
{
}


BaseObjectPtr MarkerFormat::clone()
{
	return BaseObjectPtr(new MarkerFormat(*this));
}


void MarkerFormat::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 4, fNotShowInt);
	SETBIT(flags, 5, fNotShowBrd);
	record << rgbFore << rgbBack << imk << flags << icvFore << icvBack << miSize;
}


void MarkerFormat::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> rgbFore >> rgbBack >> imk >> flags >> icvFore >> icvBack >> miSize;
	fAuto = GETBIT(flags, 0);
	fNotShowInt = GETBIT(flags, 4);
	fNotShowBrd = GETBIT(flags, 5);
}

} // namespace XLS

