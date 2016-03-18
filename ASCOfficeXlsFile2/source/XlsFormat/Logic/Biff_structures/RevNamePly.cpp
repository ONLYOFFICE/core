
#include "RevNamePly.h"
#include <Binary/CFRecord.h>

namespace XLS
{


RevNamePly::RevNamePly()
{
}


RevNamePly::RevNamePly(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevNamePly::clone()
{
	return BiffStructurePtr(new RevNamePly(*this));
}


void RevNamePly::store(CFRecord& record)
{
	record << sheet << name;
}


void RevNamePly::load(CFRecord& record)
{
	record >> sheet >> name;
}


} // namespace XLS

