
#include "Feat11FdaAutoFilter.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Feat11FdaAutoFilter::clone()
{
	return BiffStructurePtr(new Feat11FdaAutoFilter(*this));
}


void Feat11FdaAutoFilter::store(CFRecord& record)
{	
}


void Feat11FdaAutoFilter::load(CFRecord& record)
{	
	record >> cbAutoFilter;	
	record.skipNunBytes(2);	

	_UINT32 size = cbAutoFilter;
	if (cbAutoFilter)
		recAutoFilter.readFields(record);
}

} // namespace XLS

