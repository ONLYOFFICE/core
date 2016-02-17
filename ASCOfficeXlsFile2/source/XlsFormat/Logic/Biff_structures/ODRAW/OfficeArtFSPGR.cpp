
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

	_x	= (std::min)(xLeft, xRight);
	_cx = (std::max)(xLeft, xRight) - _x;
	
	_y	= (std::min)(yTop, yBottom) ;	
	_cy = (std::max)(yTop, yBottom) - _y;	
}


} // namespace XLS
