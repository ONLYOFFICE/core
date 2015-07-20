
#include "FnGroupName.h"

namespace XLS
{;

FnGroupName::FnGroupName()
{
}


FnGroupName::~FnGroupName()
{
}


BaseObjectPtr FnGroupName::clone()
{
	return BaseObjectPtr(new FnGroupName(*this));
}


void FnGroupName::writeFields(CFRecord& record)
{
	record << rgch;
}


void FnGroupName::readFields(CFRecord& record)
{
	record >> rgch;
}

} // namespace XLS

