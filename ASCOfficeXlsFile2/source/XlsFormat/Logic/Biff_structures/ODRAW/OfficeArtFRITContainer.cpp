
#include "OfficeArtFRITContainer.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtFRITContainer::OfficeArtFRITContainer()
:	OfficeArtRecord(0, FRITContainer)
{
}


XLS::BiffStructurePtr OfficeArtFRITContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFRITContainer(*this));
}


void OfficeArtFRITContainer::storeFields(XLS::CFRecord& record)
{
	record << rgfrit;
}


void OfficeArtFRITContainer::loadFields(XLS::CFRecord& record)
{
	size_t frits_start = record.getRdPtr();
	while(record.getRdPtr() < frits_start + rh_own.recLen)
	{
		OfficeArtFRIT frit;
		record >> frit;
		rgfrit.push_back(frit);
	}
}

const unsigned short OfficeArtFRITContainer::GetInstanceToStore()
{
	return rgfrit.size();
}

} // namespace XLS
