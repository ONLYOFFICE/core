
#include "PhRuns.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PhRuns::clone()
{
	return BiffStructurePtr(new PhRuns(*this));
}

void PhRuns::store(CFRecord& record)
{
	record << ichFirst << ichMom << cchMom;
}


void PhRuns::load(CFRecord& record)
{
	record >> ichFirst >> ichMom >> cchMom;
}


} // namespace XLS

