
#include "DropBar.h"

namespace XLS
{

DropBar::DropBar()
{
}


DropBar::~DropBar()
{
}


BaseObjectPtr DropBar::clone()
{
	return BaseObjectPtr(new DropBar(*this));
}


void DropBar::writeFields(CFRecord& record)
{
	record << pcGap;
}


void DropBar::readFields(CFRecord& record)
{
	record >> pcGap;
}

} // namespace XLS

