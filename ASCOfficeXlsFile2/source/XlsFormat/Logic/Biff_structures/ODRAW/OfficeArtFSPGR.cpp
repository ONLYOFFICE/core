
#include "OfficeArtFSPGR.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtFSPGR::OfficeArtFSPGR()
:	OfficeArtRecord(0x01, FSPGR)
{
}


XLS::BiffStructurePtr OfficeArtFSPGR::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFSPGR(*this));
}


void OfficeArtFSPGR::storeFields(XLS::CFRecord& record)
{
	record << xLeft << yTop << xRight << yBottom;
}


void OfficeArtFSPGR::loadFields(XLS::CFRecord& record)
{
	record >> xLeft >> yTop >> xRight >> yBottom;
}


} // namespace XLS
