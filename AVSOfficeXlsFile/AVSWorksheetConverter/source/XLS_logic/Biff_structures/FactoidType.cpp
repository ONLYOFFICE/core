#include "stdafx.h"
#include "FactoidType.h"
#include <XLS_bin/CFRecord.h>

namespace OSHARED
{;


XLS::BiffStructurePtr FactoidType::clone()
{
	return XLS::BiffStructurePtr(new FactoidType(*this));
}


void FactoidType::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"id", id);
	own_tag->setAttribute(L"rgbUri", rgbUri);
	own_tag->setAttribute(L"rgbTag", rgbTag);
	own_tag->setAttribute(L"rgbDownLoadURL", rgbDownLoadURL);
}


const bool FactoidType::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### FactoidType record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FactoidType record has no BiffStructure::fromXML() implemented.");
	return false;
}


void FactoidType::store(XLS::CFRecord& record)
{
#pragma message("####################### FactoidType record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FactoidType record has no BiffStructure::store() implemented.");
	//record << something;
}


void FactoidType::load(XLS::CFRecord& record)
{
	DWORD cbFactoid;
	record >> cbFactoid >> id >> rgbUri >> rgbTag >> rgbDownLoadURL;
}


} // namespace OSHARED

