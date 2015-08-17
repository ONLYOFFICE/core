
#include "DXFId.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


DXFId::DXFId()
{
}


DXFId::DXFId(CFRecord& record)
{
	load(record);
}


BiffStructurePtr DXFId::clone()
{
	return BiffStructurePtr(new DXFId(*this));
}


void DXFId::store(CFRecord& record)
{
	record << index;
}


void DXFId::load(CFRecord& record)
{
	record >> index;
}


} // namespace XLS

