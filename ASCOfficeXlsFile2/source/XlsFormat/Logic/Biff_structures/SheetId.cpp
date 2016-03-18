
#include "SheetId.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr SheetId::clone()
{
	return BiffStructurePtr(new SheetId(*this));
}

void SheetId::store(CFRecord& record)
{
	record << id;
}


void SheetId::load(CFRecord& record)
{
	record >> id;
}


} // namespace XLS

