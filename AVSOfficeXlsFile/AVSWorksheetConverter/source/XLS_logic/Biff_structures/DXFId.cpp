#include "stdafx.h"
#include "DXFId.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


DXFId::DXFId()
{
}


DXFId::DXFId(CFRecord& record)
{
	load(record);
}


BiffStructurePtr DXFId::clone()
{
	return BiffStructurePtr(new DXFId(*this));
}


void DXFId::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"index", index);
}


const bool DXFId::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### DXFId record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! DXFId record has no BiffStructure::fromXML() implemented.");
	return false;
}


void DXFId::store(CFRecord& record)
{
	record << index;
}


void DXFId::load(CFRecord& record)
{
	record >> index;
}


} // namespace XLS

