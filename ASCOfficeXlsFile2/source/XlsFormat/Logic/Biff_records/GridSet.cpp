#include "precompiled_xls.h"
#include "GridSet.h"

namespace XLS
{;

GridSet::GridSet()
{
}


GridSet::~GridSet()
{
}


BaseObjectPtr GridSet::clone()
{
	return BaseObjectPtr(new GridSet(*this));
}


void GridSet::writeFields(CFRecord& record)
{
	unsigned short reserved = 1;
	record << reserved;
}


void GridSet::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
}

} // namespace XLS

