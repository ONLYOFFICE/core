
#include "OfficeArtSplitMenuColorContainer.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtSplitMenuColorContainer::OfficeArtSplitMenuColorContainer()
:	OfficeArtRecord(0, SplitMenuColorContainer)
{
}


XLS::BiffStructurePtr OfficeArtSplitMenuColorContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtSplitMenuColorContainer(*this));
}

void OfficeArtSplitMenuColorContainer::storeFields(XLS::CFRecord& record)
{
//	record << smca;
}


void OfficeArtSplitMenuColorContainer::loadFields(XLS::CFRecord& record)
{
	for(size_t i = 0; i < rh_own.recInstance; ++i)
	{
		MSOCR color;
		record >> color;
		smca.push_back(color);
	}
}

const unsigned short OfficeArtSplitMenuColorContainer::GetInstanceToStore()
{
	return smca.size();
}

} // namespace XLS
