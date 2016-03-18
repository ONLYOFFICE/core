
#include "ISSTInf.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ISSTInf::clone()
{
	return BiffStructurePtr(new ISSTInf(*this));
}


void ISSTInf::store(CFRecord& record)
{
	record << ib << cbOffset;
	record.reserveNunBytes(2); // reserved
}


void ISSTInf::load(CFRecord& record)
{
	record >> ib >> cbOffset;
	record.skipNunBytes(2); // reserved
}


} // namespace XLS

