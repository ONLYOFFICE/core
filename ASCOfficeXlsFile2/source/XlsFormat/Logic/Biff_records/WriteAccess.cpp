
#include "WriteAccess.h"

namespace XLS
{

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
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		ShortXLAnsiString name;
		record >> name;

		userName = name;
	}
	else
	{
		record >> userName;
	}
	record.skipNunBytes(record.getDataSize() - record.getRdPtr()); // unused
}

} // namespace XLS

