#include "stdafx.h"
#include "Property.h"
#include <XLS_bin/CFRecord.h>

namespace OSHARED
{;


XLS::BiffStructurePtr Property::clone()
{
	return XLS::BiffStructurePtr(new Property(*this));
}


void Property::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"keyIndex", keyIndex);
	own_tag->setAttribute(L"valueIndex", valueIndex);
}


const bool Property::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### Property record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! Property record has no BiffStructure::fromXML() implemented.");
	return false;
}


void Property::store(XLS::CFRecord& record)
{
#pragma message("####################### Property record has no BiffStructure::store() implemented")
	Log::error(" Error!!! Property record has no BiffStructure::store() implemented.");
	//record << something;
}


void Property::load(XLS::CFRecord& record)
{
	record >> keyIndex >> valueIndex;
}


} // namespace OSHARED

