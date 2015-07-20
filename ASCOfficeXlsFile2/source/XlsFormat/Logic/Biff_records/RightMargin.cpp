
#include "RightMargin.h"

namespace XLS
{;

RightMargin::RightMargin()
{
}


RightMargin::~RightMargin()
{
}


BaseObjectPtr RightMargin::clone()
{
	return BaseObjectPtr(new RightMargin(*this));
}


void RightMargin::writeFields(CFRecord& record)
{
	record << num;
}


void RightMargin::readFields(CFRecord& record)
{
	record >> num;
}

} // namespace XLS

