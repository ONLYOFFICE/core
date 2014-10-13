#include "precompiled_xls.h"
#include "XFPropGradientStop.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropGradientStop::clone()
{
	return BiffStructurePtr(new XFPropGradientStop(*this));
}

//
//void XFPropGradientStop::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//	own_tag->setAttribute(L"numPosition", numPosition);
//	color.toXML(own_tag);
//}
//
//const bool XFPropGradientStop::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### XFPropGradientStop record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! XFPropGradientStop record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void XFPropGradientStop::store(CFRecord& record)
{
#pragma message("####################### XFPropGradientStop record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropGradientStop record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropGradientStop::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused
	record >> numPosition >> color;
}


} // namespace XLS
