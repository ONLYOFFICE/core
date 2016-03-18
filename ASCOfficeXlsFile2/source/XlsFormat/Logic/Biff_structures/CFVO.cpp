
#include "CFVO.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CFVO::clone()
{
	return BiffStructurePtr(new CFVO(*this));
}


void CFVO::store(CFRecord& record)
{
	record << cfvoType;
	fmla.store(record);
	if(!fmla.getCCE())
	{
		record << numValue;
	}
}


void CFVO::load(CFRecord& record)
{
	record >> cfvoType;
	fmla.load(record);
	if(!fmla.getCCE())
	{
		record >> numValue;
	}
}


} // namespace XLS

