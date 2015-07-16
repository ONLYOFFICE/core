#include "precompiled_xls.h"
#include "RadarArea.h"

namespace XLS
{;

RadarArea::RadarArea()
{
}


RadarArea::~RadarArea()
{
}


BaseObjectPtr RadarArea::clone()
{
	return BaseObjectPtr(new RadarArea(*this));
}


void RadarArea::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNunBytes(2); // unused
}


void RadarArea::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fRdrAxLab = GETBIT(flags, 0);
	fHasShadow = GETBIT(flags, 1);
	record.skipNunBytes(2); // unused
}

} // namespace XLS

