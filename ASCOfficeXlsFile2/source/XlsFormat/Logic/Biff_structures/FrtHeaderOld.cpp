
#include "FrtHeaderOld.h"
#include <Binary/CFRecord.h>

namespace XLS
{


FrtHeaderOld::FrtHeaderOld()
{
}

FrtHeaderOld::FrtHeaderOld(const CFRecordType::TypeId type)
:	rt(type)
{
}

BiffStructurePtr FrtHeaderOld::clone()
{
	return BiffStructurePtr(new FrtHeaderOld(*this));
}


void FrtHeaderOld::store(CFRecord& record)
{
	grbitFrt.fFrtRef = 0;
	grbitFrt.fFrtAlert = 0;
	record << rt << grbitFrt;
}


void FrtHeaderOld::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
}


} // namespace XLS

