#include "precompiled_xls.h"
#include "FeatSmartTag.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


FeatSmartTag::FeatSmartTag()
{
}


FeatSmartTag::FeatSmartTag(CFRecord& record)
{
	load(record);
}


BiffStructurePtr FeatSmartTag::clone()
{
	return BiffStructurePtr(new FeatSmartTag(*this));
}


//void FeatSmartTag::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"hashValue", hashValue);
//	std::for_each(rgFactoid.begin(), rgFactoid.end(), boost::bind(&FactoidData::toXML, _1, own_tag));
//}


//const bool FeatSmartTag::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### FeatSmartTag record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! FeatSmartTag record has no BiffStructure::fromXML() implemented.");
//	return false;
//}
//

void FeatSmartTag::store(CFRecord& record)
{
#pragma message("####################### FeatSmartTag record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FeatSmartTag record has no BiffStructure::store() implemented.");
	//record << something;
}


void FeatSmartTag::load(CFRecord& record)
{
	unsigned char cSmartTags;
	record >> hashValue >> cSmartTags;
	for (int i = 0; i < cSmartTags ; ++i)
	{
		FactoidData data;
		record >> data;
		rgFactoid.push_back(data);
	}
}


} // namespace XLS

