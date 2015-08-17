
#include "ChartFormat.h"

namespace XLS
{

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
	record.reserveNunBytes(16); // reserved
	unsigned short flags = 0;
	SETBIT(flags, 0, fVaried);
	record << flags << icrt;
}


void ChartFormat::readFields(CFRecord& record)
{
	record.skipNunBytes(16); // reserved
	unsigned short flags;
	record >> flags >> icrt;
	fVaried = GETBIT(flags, 0);
}

} // namespace XLS

