
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
	record.reserveNunBytes(2, static_cast<unsigned short>(1)); // reserved
}


void UserSViewEnd::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
}

} // namespace XLS

