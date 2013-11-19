#include "stdafx.h"
#include "FactoidData.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


FactoidData::FactoidData()
{
}


FactoidData::FactoidData(CFRecord& record)
{
	load(record);
}


BiffStructurePtr FactoidData::clone()
{
	return BiffStructurePtr(new FactoidData(*this));
}


void FactoidData::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"fDelete", fDelete);
	own_tag->setAttribute(L"fXMLBased", fXMLBased);
	propertyBag.toXML(own_tag);
}


const bool FactoidData::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### FactoidData record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FactoidData record has no BiffStructure::fromXML() implemented.");
	return false;
}


void FactoidData::store(CFRecord& record)
{
#pragma message("####################### FactoidData record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FactoidData record has no BiffStructure::store() implemented.");
	//record << something;
}


void FactoidData::load(CFRecord& record)
{
	WORD flags;
	record >> flags;
	fDelete = GETBIT(flags, 0);
	fXMLBased = GETBIT(flags, 0);
	record >> propertyBag;
}


} // namespace XLS

