
#include "HorzBrk.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr HorzBrk::clone()
{
	return BiffStructurePtr(new HorzBrk(*this));
}


void HorzBrk::store(CFRecord& record)
{
	record << row << colStart << colEnd;
}


void HorzBrk::load(CFRecord& record)
{
	record >> row >> colStart >> colEnd;
}


} // namespace XLS

