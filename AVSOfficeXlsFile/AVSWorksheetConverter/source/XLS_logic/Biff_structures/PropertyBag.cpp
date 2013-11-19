#include "stdafx.h"
#include "PropertyBag.h"
#include <XLS_bin/CFRecord.h>

namespace OSHARED
{;


XLS::BiffStructurePtr PropertyBag::clone()
{
	return XLS::BiffStructurePtr(new PropertyBag(*this));
}


void PropertyBag::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"id", id);
	std::for_each(properties.begin(), properties.end(), boost::bind(&Property::toXML, _1, own_tag));
}


const bool PropertyBag::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### PropertyBag record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! PropertyBag record has no BiffStructure::fromXML() implemented.");
	return false;
}


void PropertyBag::store(XLS::CFRecord& record)
{
#pragma message("####################### PropertyBag record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PropertyBag record has no BiffStructure::store() implemented.");
	//record << something;
}


void PropertyBag::load(XLS::CFRecord& record)
{
	WORD cProp;
	record >> id >> cProp;
	record.skipNBytes(2); // ignored
	for (int i = 0; i < cProp ; i++)
	{
		Property prop;
		record >> prop;
		properties.push_back(prop);
	}
}


} // namespace OSHARED

