
#include "WriteAccess.h"

namespace XLS
{;

WriteAccess::WriteAccess()
{
}


WriteAccess::~WriteAccess()
{
}


BaseObjectPtr WriteAccess::clone()
{
	return BaseObjectPtr(new WriteAccess(*this));
}


void WriteAccess::writeFields(CFRecord& record)
{
	record << userName;
	record.reserveNunBytes(112 - userName.getStructSize(), static_cast<unsigned char>(' ')); // unused and shall be filled in with spaces
}


void WriteAccess::readFields(CFRecord& record)
{
	record >> userName;
	record.skipNunBytes(112 - userName.getStructSize()); // unused
}

} // namespace XLS

