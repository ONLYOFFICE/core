
#include "DropDownObjId.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DropDownObjId::clone()
{
	return BiffStructurePtr(new DropDownObjId(*this));
}


void DropDownObjId::store(CFRecord& record)
{
	record << id;
}


void DropDownObjId::load(CFRecord& record)
{
	record >> id;
}


} // namespace XLS

