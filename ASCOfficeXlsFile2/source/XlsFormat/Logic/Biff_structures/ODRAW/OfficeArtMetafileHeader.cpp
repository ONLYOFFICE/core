
#include "OfficeArtMetafileHeader.h"
#include <Binary/CFRecord.h>
#include "OfficeArtBlip.h"

namespace ODRAW
{


XLS::BiffStructurePtr OfficeArtMetafileHeader::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtMetafileHeader(*this));
}



void OfficeArtMetafileHeader::store(XLS::CFRecord& record)
{
	// We don't support xlsx to xls conversion
}


void OfficeArtMetafileHeader::load(XLS::CFRecord& record)
{
	record >> cbSize;
	if (record.loadAnyData(rcBounds) == false)	return;
	if (record.loadAnyData(ptSize) == false)	return;
	
	record >> cbSave >> compression >> filter;
}





} // namespace XLS
