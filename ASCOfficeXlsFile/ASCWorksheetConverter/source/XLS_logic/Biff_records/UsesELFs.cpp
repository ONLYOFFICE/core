#include "stdafx.h"
#include "UsesELFs.h"

namespace XLS
{;

UsesELFs::UsesELFs()
{
}


UsesELFs::~UsesELFs()
{
}


BaseObjectPtr UsesELFs::clone()
{
	return BaseObjectPtr(new UsesELFs(*this));
}


void UsesELFs::writeFields(CFRecord& record)
{
	record << useselfs;
}


void UsesELFs::readFields(CFRecord& record)
{
	record >> useselfs;
}

} // namespace XLS

