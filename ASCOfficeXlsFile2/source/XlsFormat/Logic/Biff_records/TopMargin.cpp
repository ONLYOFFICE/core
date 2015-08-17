
#include "TopMargin.h"

namespace XLS
{

TopMargin::TopMargin()
{
}


TopMargin::~TopMargin()
{
}


BaseObjectPtr TopMargin::clone()
{
	return BaseObjectPtr(new TopMargin(*this));
}


void TopMargin::writeFields(CFRecord& record)
{
	record << num;
}


void TopMargin::readFields(CFRecord& record)
{
	record >> num;
}

} // namespace XLS

