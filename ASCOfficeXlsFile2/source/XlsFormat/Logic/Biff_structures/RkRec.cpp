
#include "RkRec.h"
#include <Binary/CFRecord.h>

namespace XLS
{
extern int cellStyleXfs_count;

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
	record >> ixfe >> RK_;
}


const unsigned short RkRec::get_ixfe() const
{
	return ixfe;
}

} // namespace XLS

