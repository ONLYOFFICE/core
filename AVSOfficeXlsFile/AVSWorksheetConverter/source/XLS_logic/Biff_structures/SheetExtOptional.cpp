#include "stdafx.h"
#include "SheetExtOptional.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


SheetExtOptional::SheetExtOptional()
{
}


SheetExtOptional::SheetExtOptional(CFRecord& record)
{
	load(record);
}


BiffStructurePtr SheetExtOptional::clone()
{
	return BiffStructurePtr(new SheetExtOptional(*this));
}


void SheetExtOptional::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"icvPlain12", icvPlain12);
	own_tag->setAttribute(L"fCondFmtCalc", fCondFmtCalc);
	own_tag->setAttribute(L"fNotPublished", fNotPublished);
	color.toXML(own_tag);
}


const bool SheetExtOptional::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### SheetExtOptional record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! SheetExtOptional record has no BiffStructure::fromXML() implemented.");
	return false;
}


void SheetExtOptional::store(CFRecord& record)
{
#pragma message("####################### SheetExtOptional record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SheetExtOptional record has no BiffStructure::store() implemented.");
	//record << something;
}


void SheetExtOptional::load(CFRecord& record)
{
	DWORD flags;
	record >> flags;
	icvPlain12 = static_cast<BYTE>(GETBITS(flags, 0, 6));
	fCondFmtCalc = GETBIT(flags, 7);
	fNotPublished = GETBIT(flags, 8);
	record >> color;
}


} // namespace XLS

