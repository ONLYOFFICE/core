#include "stdafx.h"
#include "XFPropGradientStop.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropGradientStop::clone()
{
	return BiffStructurePtr(new XFPropGradientStop(*this));
}


void XFPropGradientStop::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	own_tag->setAttribute(L"numPosition", numPosition);
	color.toXML(own_tag);
}

const bool XFPropGradientStop::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### XFPropGradientStop record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! XFPropGradientStop record has no BiffStructure::fromXML() implemented.");
	return false;
}


void XFPropGradientStop::store(CFRecord& record)
{
#pragma message("####################### XFPropGradientStop record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradientStop record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradientStop::load(CFRecord& record)
{
	record.skipNBytes(2); // unused
	record >> numPosition >> color;
}


} // namespace XLS
