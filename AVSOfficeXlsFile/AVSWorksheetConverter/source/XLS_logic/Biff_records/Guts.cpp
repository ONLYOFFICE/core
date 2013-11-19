#include "stdafx.h"
#include "Guts.h"

namespace XLS
{;

Guts::Guts()
{
}


Guts::~Guts()
{
}


BaseObjectPtr Guts::clone()
{
	return BaseObjectPtr(new Guts(*this));
}


void Guts::writeFields(CFRecord& record)
{
	record.reserveNBytes(4); // unused
	record << iLevelRwMac << iLevelColMac;
}


void Guts::readFields(CFRecord& record)
{
	record.skipNBytes(4); // unused
	record >> iLevelRwMac >> iLevelColMac;
}

} // namespace XLS

