#include "precompiled_xls.h"
#include "Mms.h"
#include <Binary/CFStream.h>

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
	record.reserveNunBytes(2); // reserved
}


void Mms::readFields(CFRecord& record)
{
	// This record is reserved and MUST be ignored.
	record.skipNunBytes(2); // reserved
}

} // namespace XLS
