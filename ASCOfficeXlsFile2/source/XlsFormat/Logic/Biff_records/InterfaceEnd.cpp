
#include "InterfaceEnd.h"


namespace XLS
{

InterfaceEnd::InterfaceEnd()
{
}


InterfaceEnd::~InterfaceEnd()
{
}


BaseObjectPtr InterfaceEnd::clone()
{
	return BaseObjectPtr(new InterfaceEnd(*this));
}


void InterfaceEnd::writeFields(CFRecord& record)
{
	// No data in this record
}


void InterfaceEnd::readFields(CFRecord& record)
{
	// No data in this record
}


} // namespace XLS
