#include "precompiled_xls.h"
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
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fRdrAxLab);
	SETBIT(flags, 1, fHasShadow);
	record << flags;
	record.reserveNunBytes(2); // unused
}


void Radar::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> flags;
	fRdrAxLab = GETBIT(flags, 0);
	fHasShadow = GETBIT(flags, 1);
	record.skipNunBytes(2); // unused
}

} // namespace XLS

