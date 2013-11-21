#include "stdafx.h"
#include "FeatSmartTag.h"
#include <XLS_bin/CFRecord.h>

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


void FeatSmartTag::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"hashValue", hashValue);
	std::for_each(rgFactoid.begin(), rgFactoid.end(), boost::bind(&FactoidData::toXML, _1, own_tag));
}


const bool FeatSmartTag::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### FeatSmartTag record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FeatSmartTag record has no BiffStructure::fromXML() implemented.");
	return false;
}


void FeatSmartTag::store(CFRecord& record)
{
#pragma message("####################### FeatSmartTag record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FeatSmartTag record has no BiffStructure::store() implemented.");
	//record << something;
}


void FeatSmartTag::load(CFRecord& record)
{
	BYTE cSmartTags;
	record >> hashValue >> cSmartTags;
	for (int i = 0; i < cSmartTags ; ++i)
	{
		FactoidData data;
		record >> data;
		rgFactoid.push_back(data);
	}
}


} // namespace XLS

