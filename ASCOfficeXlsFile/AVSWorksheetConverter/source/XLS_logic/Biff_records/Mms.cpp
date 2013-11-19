#include "stdafx.h"
#include "Mms.h"
#include <XLS_bin/CFStream.h>

namespace XLS
{;


Mms::Mms()
{
}


Mms::~Mms()
{
}


BaseObjectPtr Mms::clone()
{
	return BaseObjectPtr(new Mms(*this));
}


void Mms::writeFields(CFRecord& record)
{
	// This record is reserved and MUST be ignored.
	record.reserveNBytes(2); // reserved
}


void Mms::readFields(CFRecord& record)
{
	// This record is reserved and MUST be ignored.
	record.skipNBytes(2); // reserved
}

} // namespace XLS
