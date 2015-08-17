
#include "PhoneticInfo.h"

namespace XLS
{

PhoneticInfo::PhoneticInfo()
{
}


PhoneticInfo::~PhoneticInfo()
{
}


BaseObjectPtr PhoneticInfo::clone()
{
	return BaseObjectPtr(new PhoneticInfo(*this));
}


void PhoneticInfo::writeFields(CFRecord& record)
{
	record << phs << sqref;
}


void PhoneticInfo::readFields(CFRecord& record)
{
	record >> phs >> sqref;
}

} // namespace XLS

