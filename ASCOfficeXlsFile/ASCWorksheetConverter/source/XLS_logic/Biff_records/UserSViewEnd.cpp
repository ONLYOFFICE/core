#include "stdafx.h"
#include "UserSViewEnd.h"

namespace XLS
{;

UserSViewEnd::UserSViewEnd()
{
}


UserSViewEnd::~UserSViewEnd()
{
}


BaseObjectPtr UserSViewEnd::clone()
{
	return BaseObjectPtr(new UserSViewEnd(*this));
}


void UserSViewEnd::writeFields(CFRecord& record)
{
	record.reserveNBytes(2, static_cast<WORD>(1)); // reserved
}


void UserSViewEnd::readFields(CFRecord& record)
{
	record.skipNBytes(2); // reserved
}

} // namespace XLS

