
#include "WriteProtect.h"
#include <Binary/CFStream.h>

namespace XLS
{


WriteProtect::WriteProtect()
{
}


WriteProtect::~WriteProtect()
{
}


BaseObjectPtr WriteProtect::clone()
{
	return BaseObjectPtr(new WriteProtect(*this));
}


void WriteProtect::writeFields(CFRecord& record)
{
	// No data in this record
}


void WriteProtect::readFields(CFRecord& record)
{
	// No data in this record
}


} // namespace XLS
