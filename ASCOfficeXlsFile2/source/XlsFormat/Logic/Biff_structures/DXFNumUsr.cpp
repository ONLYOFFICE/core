
#include "DXFNumUsr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


DXFNumUsr::DXFNumUsr()
{
}


BiffStructurePtr DXFNumUsr::clone()
{
	return BiffStructurePtr(new DXFNumUsr(*this));
}

void DXFNumUsr::store(CFRecord& record)
{
	size_t start_ptr = record.getDataSize();
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cb*/, rt_STRUCT_DXFNumUsr);
	record << fmt;
	record.registerDelayedDataSource(record.getDataSize() - start_ptr, rt_STRUCT_DXFNumUsr);

}


void DXFNumUsr::load(CFRecord& record)
{
	record >> cb >> fmt;
}


} // namespace XLS

