#include "stdafx.h"
#include "Radar.h"

namespace XLS
{;

Radar::Radar()
{
}


Radar::~Radar()
{
}


BaseObjectPtr Radar::clone()
{
	return BaseObjectPtr(new Radar(*this));
}


void Radar::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNBytes(2); // unused
}


void Radar::readFields(CFRecord& record)
{
	WORD flags;
	record >> flags;
	fRdrAxLab = GETBIT(flags, 0);
	fHasShadow = GETBIT(flags, 1);
	record.skipNBytes(2); // unused
}

} // namespace XLS

