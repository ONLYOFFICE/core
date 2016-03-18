
#include "OfficeArtClientAnchorChart.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtClientAnchorChart::OfficeArtClientAnchorChart()
:	OfficeArtRecord(0x00, ClientAnchor)
{
}


XLS::BiffStructurePtr OfficeArtClientAnchorChart::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorChart(*this));
}



void OfficeArtClientAnchorChart::storeFields(XLS::CFRecord& record)
{
	unsigned short flags = 0;
// 	SETBIT(flags, 0, fMove);
	SETBIT(flags, 1, fSize);
	record << flags << lx1 << ly1 << lx2 << ly2;
}


void OfficeArtClientAnchorChart::loadFields(XLS::CFRecord& record)
{
	unsigned short flags;
	record >> flags >> lx1 >> ly1 >> lx2 >> ly2;
// 	fMove = GETBIT(flags, 0);
	fSize = GETBIT(flags, 1);

}


} // namespace XLS
