
#include "OfficeArtFSP.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtFSP::OfficeArtFSP()
:	OfficeArtRecord(0x02, FSP)
{
}


XLS::BiffStructurePtr OfficeArtFSP::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFSP(*this));
}


void OfficeArtFSP::storeFields(XLS::CFRecord& record)
{
	unsigned int flags = 0;
	SETBIT(flags, 0, fGroup);
	SETBIT(flags, 1, fChild);
	SETBIT(flags, 2, fPatriarch);
	SETBIT(flags, 3, fDeleted);
	SETBIT(flags, 4, fOleShape);
	SETBIT(flags, 5, fHaveMaster);
	SETBIT(flags, 6, fFlipH);
	SETBIT(flags, 7, fFlipV);
	SETBIT(flags, 8, fConnector);
	SETBIT(flags, 9, fHaveAnchor);
	SETBIT(flags, 10, fBackground);
	SETBIT(flags, 11, fHaveSpt);

	record << spid << flags;
}


void OfficeArtFSP::loadFields(XLS::CFRecord& record)
{
	unsigned int flags;
	record >> spid >> flags;

	fGroup = GETBIT(flags, 0);
	fChild = GETBIT(flags, 1);
	fPatriarch = GETBIT(flags, 2);
	fDeleted = GETBIT(flags, 3);
	fOleShape = GETBIT(flags, 4);
	fHaveMaster = GETBIT(flags, 5);
	fFlipH = GETBIT(flags, 6);
	fFlipV = GETBIT(flags, 7);
	fConnector = GETBIT(flags, 8);
	fHaveAnchor = GETBIT(flags, 9);
	fBackground = GETBIT(flags, 10);
	fHaveSpt = GETBIT(flags, 11);

	shape_id = rh_own.recInstance;
}


const unsigned short OfficeArtFSP::GetInstanceToStore()
{
	return shape_id;
}


} // namespace XLS
