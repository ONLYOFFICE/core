
#include "SerParent.h"

namespace XLS
{

SerParent::SerParent()
{
}


SerParent::~SerParent()
{
}


BaseObjectPtr SerParent::clone()
{
	return BaseObjectPtr(new SerParent(*this));
}


void SerParent::writeFields(CFRecord& record)
{
	record << series;
}


void SerParent::readFields(CFRecord& record)
{
	record >> series;
}

} // namespace XLS

