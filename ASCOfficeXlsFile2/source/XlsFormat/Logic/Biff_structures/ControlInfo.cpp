
#include "ControlInfo.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ControlInfo::clone()
{
	return BiffStructurePtr(new ControlInfo(*this));
}


void ControlInfo::store(CFRecord& record)
{	
}

void ControlInfo::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;

	fDefault = GETBIT(flags, 0);
	fHelp = GETBIT(flags, 1);
	fCancel = GETBIT(flags, 2);
	fDismiss = GETBIT(flags, 3);

	record >> accel1;
	record.skipNunBytes(2);	// reserved2
}


} // namespace XLS

