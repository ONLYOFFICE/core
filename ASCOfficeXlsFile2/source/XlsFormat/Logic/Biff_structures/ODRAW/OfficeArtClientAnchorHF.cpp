
#include "OfficeArtClientAnchorHF.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtClientAnchorHF::OfficeArtClientAnchorHF()
:	OfficeArtRecord(0x00, ClientAnchor)
{
}


XLS::BiffStructurePtr OfficeArtClientAnchorHF::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorHF(*this));
}

void OfficeArtClientAnchorHF::storeFields(XLS::CFRecord& record)
{
	record << width << height;
}


void OfficeArtClientAnchorHF::loadFields(XLS::CFRecord& record)
{
	record >> width >> height;
}


} // namespace XLS
