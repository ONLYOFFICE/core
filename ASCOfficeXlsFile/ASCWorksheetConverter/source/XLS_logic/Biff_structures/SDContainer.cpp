#include "stdafx.h"
#include "SDContainer.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


SDContainer::SDContainer()
{
}


SDContainer::SDContainer(CFRecord& record)
{
	load(record);
}


BiffStructurePtr SDContainer::clone()
{
	return BiffStructurePtr(new SDContainer(*this));
}


void SDContainer::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	sd.toXML(parent);
}


const bool SDContainer::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### SDContainer record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! SDContainer record has no BiffStructure::fromXML() implemented.");
	return false;
}


void SDContainer::store(CFRecord& record)
{
#pragma message("####################### SDContainer record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SDContainer record has no BiffStructure::store() implemented.");
	//record << something;
}


void SDContainer::load(CFRecord& record)
{
	DWORD cbSD;
	record >> cbSD >> sd;
}


} // namespace XLS

