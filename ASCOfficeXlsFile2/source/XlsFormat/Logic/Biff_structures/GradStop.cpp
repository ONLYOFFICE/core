#include "precompiled_xls.h"
#include "GradStop.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr GradStop::clone()
{
	return BiffStructurePtr(new GradStop(*this));
}

//
//void GradStop::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//	own_tag->setAttribute(L"xclrType", xclrType);
//	if(2 == xclrType)
//	{
//		xclrValue_rgb.toXML(own_tag);
//	}
//	else
//	{
//		own_tag->setAttribute(L"xclrValue", xclrValue);
//	}
//	own_tag->setAttribute(L"numPosition", numPosition);
//	own_tag->setAttribute(L"numTint", numTint);
//}
//
//const bool GradStop::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### GradStop record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! GradStop record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void GradStop::store(CFRecord& record)
{
#pragma message("####################### GradStop record has no BiffStructure::store() implemented")
	Log::error(" Error!!! GradStop record has no BiffStructure::store() implemented.");
	//record << something;
}


void GradStop::load(CFRecord& record)
{
	record >> xclrType;
	if(2 == xclrType)
	{
		record >> xclrValue_rgb;
	}
	else
	{
		record >> xclrValue;
	}

	record >> numPosition >> numTint;
}


} // namespace XLS
