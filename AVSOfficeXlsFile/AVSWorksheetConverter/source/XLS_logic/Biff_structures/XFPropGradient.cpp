#include "stdafx.h"
#include "XFPropGradient.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropGradient::clone()
{
	return BiffStructurePtr(new XFPropGradient(*this));
}


void XFPropGradient::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	own_tag->setAttribute(L"type", type);
	own_tag->setAttribute(L"numDegree", numDegree);
	own_tag->setAttribute(L"numFillToLeft", numFillToLeft);
	own_tag->setAttribute(L"numFillToRight", numFillToRight);
	own_tag->setAttribute(L"numFillToTop", numFillToTop);
	own_tag->setAttribute(L"numFillToBottom", numFillToBottom);
}

const bool XFPropGradient::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### XFPropGradient record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! XFPropGradient record has no BiffStructure::fromXML() implemented.");
	return false;
}


void XFPropGradient::store(CFRecord& record)
{
#pragma message("####################### XFPropGradient record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradient record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradient::load(CFRecord& record)
{
	record >> type >> numDegree >> numFillToLeft >> numFillToRight >> 
		numFillToTop >> numFillToBottom;
}


} // namespace XLS
