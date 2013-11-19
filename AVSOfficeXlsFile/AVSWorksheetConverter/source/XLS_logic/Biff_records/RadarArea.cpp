#include "stdafx.h"
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
	WORD flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNBytes(2); // unused
}


void RadarArea::readFields(CFRecord& record)
{
	WORD flags;
	record >> flags;
	fRdrAxLab = GETBIT(flags, 0);
	fHasShadow = GETBIT(flags, 1);
	record.skipNBytes(2); // unused
}

} // namespace XLS

