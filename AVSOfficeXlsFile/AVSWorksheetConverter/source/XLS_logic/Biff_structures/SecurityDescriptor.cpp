#include "stdafx.h"
#include "SecurityDescriptor.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


SecurityDescriptor::SecurityDescriptor()
{
}


SecurityDescriptor::SecurityDescriptor(CFRecord& record)
{
	load(record);
}


BiffStructurePtr SecurityDescriptor::clone()
{
	return BiffStructurePtr(new SecurityDescriptor(*this));
}


void SecurityDescriptor::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"Revision", sd.Revision);
	own_tag->setAttribute(L"Sbz1", sd.Sbz1);
	own_tag->setAttribute(L"Control", sd.Control);
/*
	own_tag->setAttribute(L"Owner", sd.Owner); // pointer
	own_tag->setAttribute(L"Group", sd.Group); // pointer
	own_tag->setAttribute(L"Sacl", sd.Sacl); // pointer
	own_tag->setAttribute(L"Dacl", sd.Dacl); // pointer
*/
}


const bool SecurityDescriptor::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### SecurityDescriptor record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! SecurityDescriptor record has no BiffStructure::fromXML() implemented.");
	return false;
}


void SecurityDescriptor::store(CFRecord& record)
{
#pragma message("####################### SecurityDescriptor record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SecurityDescriptor record has no BiffStructure::store() implemented.");
	//record << something;
}


void SecurityDescriptor::load(CFRecord& record)
{
	record.loadAnyData(sd);
}


} // namespace XLS

