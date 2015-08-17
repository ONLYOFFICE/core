
#include "OfficeArtColorMRUContainer.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtColorMRUContainer::OfficeArtColorMRUContainer()
:	OfficeArtRecord(0, ColorMRUContainer)
{
}


XLS::BiffStructurePtr OfficeArtColorMRUContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtColorMRUContainer(*this));
}

void OfficeArtColorMRUContainer::storeFields(XLS::CFRecord& record)
{
	record << rgmsocr;
}


void OfficeArtColorMRUContainer::loadFields(XLS::CFRecord& record)
{
	for(size_t i = 0; i < rh_own.recInstance; ++i)
	{
		MSOCR color;
		record >> color;
		rgmsocr.push_back(color);
	}
}

const unsigned short OfficeArtColorMRUContainer::GetInstanceToStore()
{
	return rgmsocr.size();
}

} // namespace XLS
