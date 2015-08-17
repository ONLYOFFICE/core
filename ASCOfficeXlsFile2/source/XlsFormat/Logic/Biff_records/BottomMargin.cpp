
#include "BottomMargin.h"

namespace XLS
{

BottomMargin::BottomMargin()
{
}


BottomMargin::~BottomMargin()
{
}


BaseObjectPtr BottomMargin::clone()
{
	return BaseObjectPtr(new BottomMargin(*this));
}


void BottomMargin::writeFields(CFRecord& record)
{
	record << num;
}


void BottomMargin::readFields(CFRecord& record)
{
	record >> num;
}

} // namespace XLS

