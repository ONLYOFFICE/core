
#include "OfficeArtFDG.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{

OfficeArtFDG::OfficeArtFDG()
:	OfficeArtRecord(0, FDG)
{
}

XLS::BiffStructurePtr OfficeArtFDG::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFDG(*this));
}



void OfficeArtFDG::storeFields(XLS::CFRecord& record)
{
	record << csp << spidCur;
}


void OfficeArtFDG::loadFields(XLS::CFRecord& record)
{
	record >> csp >> spidCur;
}


} // namespace XLS
