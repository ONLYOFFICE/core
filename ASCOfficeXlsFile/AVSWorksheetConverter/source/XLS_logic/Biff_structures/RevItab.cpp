#include "stdafx.h"
#include "RevItab.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


RevItab::RevItab()
{
}


RevItab::RevItab(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevItab::clone()
{
	return BiffStructurePtr(new RevItab(*this));
}


void RevItab::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"type", type);
	switch(type)
	{
		case 0x00: 
			own_tag->setAttribute(L"tabid", tabid);
			break;
		case 0x01: 
			own_tag->setAttribute(L"sheet", sheet);
			break;
	}
}


const bool RevItab::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### RevItab record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! RevItab record has no BiffStructure::fromXML() implemented.");
	return false;
}


void RevItab::store(CFRecord& record)
{
#pragma message("####################### RevItab record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RevItab record has no BiffStructure::store() implemented.");
	//record << something;
}


void RevItab::load(CFRecord& record)
{
	record >> type;
	switch(type)
	{
		case 0x00: 
			record >> tabid;
			break;
		case 0x01: 
			record >> sheet;
			break;
	}
}


} // namespace XLS

