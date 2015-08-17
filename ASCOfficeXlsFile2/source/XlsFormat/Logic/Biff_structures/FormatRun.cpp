
#include "FormatRun.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FormatRun::clone()
{
	return BiffStructurePtr(new FormatRun(*this));
}

void FormatRun::store(CFRecord& record)
{
	record << ich << ifnt;
}


void FormatRun::load(CFRecord& record)
{
	record >> ich >> ifnt;
	unsigned short _ich = ich;
}


} // namespace XLS
