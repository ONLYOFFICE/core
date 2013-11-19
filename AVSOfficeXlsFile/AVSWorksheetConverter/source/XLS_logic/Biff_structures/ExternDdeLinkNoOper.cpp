#include "stdafx.h"
#include "ExternDdeLinkNoOper.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExternDdeLinkNoOper::clone()
{
	return BiffStructurePtr(new ExternDdeLinkNoOper(*this));
}


void ExternDdeLinkNoOper::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"linkName", linkName);
}


const bool ExternDdeLinkNoOper::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### ExternDdeLinkNoOper record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! ExternDdeLinkNoOper record has no BiffStructure::fromXML() implemented.");
	return false;
}


void ExternDdeLinkNoOper::store(CFRecord& record)
{
#pragma message("####################### ExternDdeLinkNoOper record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternDdeLinkNoOper record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternDdeLinkNoOper::load(CFRecord& record)
{
	record.skipNBytes(4); // reserved
	record >> linkName;
}


} // namespace XLS

