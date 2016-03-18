
#include "FileOffset.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FileOffset::clone()
{
	return BiffStructurePtr(new FileOffset(*this));
}


void FileOffset::store(CFRecord& record)
{
	record << offset;
}


void FileOffset::load(CFRecord& record)
{
	record >> offset;
}


} // namespace XLS

