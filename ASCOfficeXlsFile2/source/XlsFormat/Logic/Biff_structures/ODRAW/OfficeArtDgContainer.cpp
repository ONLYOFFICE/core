
#include "OfficeArtDgContainer.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;

OfficeArtDgContainer::OfficeArtDgContainer(const OfficeArtClientAnchorType anchor_type)
:	OfficeArtContainer(0x0F, DgContainer, anchor_type)
{
}


XLS::BiffStructurePtr OfficeArtDgContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtDgContainer(*this));
}


const bool OfficeArtDgContainer::CheckIfContainerStartFound(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh;
	if(!record.checkFitReadSafe(rh.size())) return false;
	record >> rh;

	return 0xF002 == rh.recType;
}


const bool OfficeArtDgContainer::CheckIfContainerSizeOK(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh;
	if(!record.checkFitReadSafe(rh.size())) return false;
	record >> rh;

	return 0xF002 == rh.recType && record.getDataSize() == rh.recLen + rh.size();
}


} // namespace ODRAW
