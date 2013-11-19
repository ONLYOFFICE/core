#include "stdafx.h"
#include "AntiMoniker.h"
#include <XLS_bin/CFRecord.h>

namespace OSHARED
{;


XLS::BiffStructurePtr AntiMoniker::clone()
{
	return XLS::BiffStructurePtr(new AntiMoniker(*this));
}


void AntiMoniker::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"count", count);
}


void AntiMoniker::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
#pragma message("####################### AntiMoniker record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! AntiMoniker record has no BiffStructure::fromXML() implemented.");
}


void AntiMoniker::store(XLS::CFRecord& record)
{
#pragma message("####################### AntiMoniker record has no BiffStructure::store() implemented")
	Log::error(" Error!!! AntiMoniker record has no BiffStructure::store() implemented.");
	//record << something;
}


void AntiMoniker::load(XLS::CFRecord& record)
{
	record >> count;
}


} // namespace OSHARED

