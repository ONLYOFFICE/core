
#include "OfficeArtFOPT.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtFOPT::OfficeArtFOPT()
:	OfficeArtRecord(0x03, FOPT)
{
}


XLS::BiffStructurePtr OfficeArtFOPT::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFOPT(*this));
}

void OfficeArtFOPT::storeFields(XLS::CFRecord& record)
{
	record << fopt;
}


void OfficeArtFOPT::loadFields(XLS::CFRecord& record)
{
	fopt.SetOptionsCount(rh_own.recInstance);
	record >> fopt;
}


const unsigned short OfficeArtFOPT::GetInstanceToStore()
{
	return fopt.GetOptionsCount();
}


} // namespace XLS
