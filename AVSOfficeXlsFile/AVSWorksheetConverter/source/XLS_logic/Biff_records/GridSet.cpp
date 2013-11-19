#include "stdafx.h"
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
	WORD reserved = 1;
	record << reserved;
}


void GridSet::readFields(CFRecord& record)
{
	record.skipNBytes(2); // reserved
}

} // namespace XLS

