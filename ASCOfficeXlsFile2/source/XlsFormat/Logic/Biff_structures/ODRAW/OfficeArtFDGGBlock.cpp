
#include "OfficeArtFDGGBlock.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtFDGGBlock::OfficeArtFDGGBlock()
:	OfficeArtRecord(0, FDGGBlock)
{
}


XLS::BiffStructurePtr OfficeArtFDGGBlock::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFDGGBlock(*this));
}


void OfficeArtFDGGBlock::storeFields(XLS::CFRecord& record)
{
	_UINT32 cidcl = Rgidcl.size();
	record << spidMax << cidcl << cspSaved << cdgSaved;
	record << Rgidcl;
}


void OfficeArtFDGGBlock::loadFields(XLS::CFRecord& record)
{
	_UINT32 cidcl;
	record >> spidMax >> cidcl >> cspSaved >> cdgSaved;

	for(size_t i = 0; i < cidcl - 1; ++i)
	{
		OfficeArtIDCL idcl;
		record >> idcl;
		Rgidcl.push_back(idcl);
	}
}

} // namespace XLS
