#include "stdafx.h"
#include "Units.h"

namespace XLS
{;

Units::Units()
{
}


Units::~Units()
{
}


BaseObjectPtr Units::clone()
{
	return BaseObjectPtr(new Units(*this));
}


void Units::writeFields(CFRecord& record)
{
	WORD reserved = 0;
	record << reserved;
}


void Units::readFields(CFRecord& record)
{
	record.skipNBytes(2); // reserved
}

} // namespace XLS

