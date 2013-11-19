#include "stdafx.h"
#include "CFExTemplateParams.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


void CFExFilterParams::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode("CFExFilterParams", parent);

	own_tag->setAttribute(L"fTop", fTop);
	own_tag->setAttribute(L"fPercent", fPercent);
	own_tag->setAttribute(L"iParam", iParam);
}


void CFExTextTemplateParams::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode("CFExTextTemplateParams", parent);

	own_tag->setAttribute(L"ctp", ctp);
}


void CFExDateTemplateParams::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode("CFExDateTemplateParams", parent);

	own_tag->setAttribute(L"dateOp", dateOp);
}


void CFExAveragesTemplateParams::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode("CFExAveragesTemplateParams", parent);

	own_tag->setAttribute(L"iParam", iParam);
}


void CFExDefaultTemplateParams::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode("CFExDefaultTemplateParams", parent);
}


BiffStructurePtr CFExTemplateParams::clone()
{
	return BiffStructurePtr(new CFExTemplateParams(*this));
}


void CFExTemplateParams::toXML(MSXML2::IXMLDOMElementPtr parent, const BYTE icfTemplate)
{
	switch(icfTemplate)
	{
		case 0x05:
			data.filter.toXML(parent);
			break;

		case 0x08:
			data.text.toXML(parent);
			break;

		case 0x0F:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
			data.date.toXML(parent);
			break;

		case 0x19:
		case 0x1A:
		case 0x1D:
		case 0x1E:
			data.averages.toXML(parent);
			break;

		default:
			data.defaults.toXML(parent);
			break;
	}
}


void CFExTemplateParams::store(CFRecord& record)
{
#pragma message("####################### CFExTemplateParams record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CFExTemplateParams record has no BiffStructure::store() implemented.");
	//record << something;
}


void CFExTemplateParams::load(CFRecord& record)
{
	record.loadAnyData(data);
}

} // namespace XLS

