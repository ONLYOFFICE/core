#include "precompiled_xls.h"
#include "XFPropBorder.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropBorder::clone()
{
	return BiffStructurePtr(new XFPropBorder(*this));
}


//void XFPropBorder::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//	own_tag->setAttribute(L"dgBorder", dgBorder);
//	color.toXML(own_tag);
//}

//const bool XFPropBorder::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### XFPropBorder record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! XFPropBorder record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void XFPropBorder::store(CFRecord& record)
{
	record << color << dgBorder;
}


void XFPropBorder::load(CFRecord& record)
{
	record >> color >> dgBorder;
}


} // namespace XLS
