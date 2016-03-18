
#include "VertBrk.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr VertBrk::clone()
{
	return BiffStructurePtr(new VertBrk(*this));
}

void VertBrk::store(CFRecord& record)
{
	record << col << rowStart << rowEnd;
}


void VertBrk::load(CFRecord& record)
{
	record >> col >> rowStart >> rowEnd;
}

} // namespace XLS

