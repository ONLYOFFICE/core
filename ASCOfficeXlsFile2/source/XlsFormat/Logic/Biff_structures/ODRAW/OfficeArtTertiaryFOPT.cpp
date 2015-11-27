
#include "OfficeArtTertiaryFOPT.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtTertiaryFOPT::OfficeArtTertiaryFOPT()
:	OfficeArtRecord(0x03, TertiaryFOPT)
{
}


XLS::BiffStructurePtr OfficeArtTertiaryFOPT::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtTertiaryFOPT(*this));
}

void OfficeArtTertiaryFOPT::storeFields(XLS::CFRecord& record)
{
	record << fopt;
}


void OfficeArtTertiaryFOPT::loadFields(XLS::CFRecord& record)
{
	fopt.SetOptionsCount(rh_own.recInstance);
	record >> fopt;
}


const unsigned short OfficeArtTertiaryFOPT::GetInstanceToStore()
{
	return fopt.GetOptionsCount();
}


} // namespace XLS
