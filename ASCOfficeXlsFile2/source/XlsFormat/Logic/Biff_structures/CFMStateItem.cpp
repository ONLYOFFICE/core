
#include "CFMStateItem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CFMStateItem::clone()
{
	return BiffStructurePtr(new CFMStateItem(*this));
}

void CFMStateItem::store(CFRecord& record)
{
	cfvo.store(record);
	record << fEqual;
	record.reserveNunBytes(4); // unused
}


void CFMStateItem::load(CFRecord& record)
{
	cfvo.load(record);
	record >> fEqual;
	record.skipNunBytes(4); // unused
}


} // namespace XLS

