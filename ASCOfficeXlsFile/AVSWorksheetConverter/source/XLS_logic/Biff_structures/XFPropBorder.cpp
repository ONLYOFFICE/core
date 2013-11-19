#include "stdafx.h"
#include "XFPropBorder.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropBorder::clone()
{
	return BiffStructurePtr(new XFPropBorder(*this));
}


void XFPropBorder::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	own_tag->setAttribute(L"dgBorder", dgBorder);
	color.toXML(own_tag);
}

const bool XFPropBorder::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### XFPropBorder record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! XFPropBorder record has no BiffStructure::fromXML() implemented.");
	return false;
}


void XFPropBorder::store(CFRecord& record)
{
	record << color << dgBorder;
}


void XFPropBorder::load(CFRecord& record)
{
	record >> color >> dgBorder;
}


} // namespace XLS
