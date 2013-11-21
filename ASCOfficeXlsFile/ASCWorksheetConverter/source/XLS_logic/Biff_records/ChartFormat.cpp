#include "stdafx.h"
#include "ChartFormat.h"

namespace XLS
{;

ChartFormat::ChartFormat()
{
}


ChartFormat::~ChartFormat()
{
}


BaseObjectPtr ChartFormat::clone()
{
	return BaseObjectPtr(new ChartFormat(*this));
}


void ChartFormat::writeFields(CFRecord& record)
{
	record.reserveNBytes(16); // reserved
	WORD flags = 0;
	SETBIT(flags, 0, fVaried);
	record << flags << icrt;
}


void ChartFormat::readFields(CFRecord& record)
{
	record.skipNBytes(16); // reserved
	WORD flags;
	record >> flags >> icrt;
	fVaried = GETBIT(flags, 0);
}

} // namespace XLS

