
#include "RkRec.h"
#include <Binary/CFRecord.h>

namespace XLS
{

BiffStructurePtr RkRec::clone()
{
	return BiffStructurePtr(new RkRec(*this));
}

void RkRec::store(CFRecord& record)
{
	record << ixfe << RK_;
}


void RkRec::load(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();
	
	record >> ixfe >> RK_;
}


const unsigned short RkRec::get_ixfe() const
{
	return ixfe;
}

} // namespace XLS

