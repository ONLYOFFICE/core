#include "precompiled_xls.h"
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
	unsigned short reserved = 0;
	record << reserved;
}


void Units::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
}

} // namespace XLS

