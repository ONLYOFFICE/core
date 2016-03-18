
#include "MDir.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr MDir::clone()
{
	return BiffStructurePtr(new MDir(*this));
}

void MDir::store(CFRecord& record)
{
	record << imdt << mdd;
}


void MDir::load(CFRecord& record)
{
	record >> imdt >> mdd;
}


} // namespace XLS
