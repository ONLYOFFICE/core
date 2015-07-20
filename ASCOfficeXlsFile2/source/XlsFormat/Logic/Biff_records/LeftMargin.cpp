
#include "LeftMargin.h"

namespace XLS
{;

LeftMargin::LeftMargin()
{
}


LeftMargin::~LeftMargin()
{
}


BaseObjectPtr LeftMargin::clone()
{
	return BaseObjectPtr(new LeftMargin(*this));
}


void LeftMargin::writeFields(CFRecord& record)
{
	record << num;
}


void LeftMargin::readFields(CFRecord& record)
{
	record >> num;
}

} // namespace XLS

