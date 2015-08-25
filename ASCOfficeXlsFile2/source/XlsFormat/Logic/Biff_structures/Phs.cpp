
#include "Phs.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Phs::clone()
{
	return BiffStructurePtr(new Phs(*this));
}


void Phs::store(CFRecord& record)
{
	record << ifnt;
	record.storeAnyData(data);
}


void Phs::load(CFRecord& record)
{
	record >> ifnt;
	record.loadAnyData(data);
}


} // namespace XLS
