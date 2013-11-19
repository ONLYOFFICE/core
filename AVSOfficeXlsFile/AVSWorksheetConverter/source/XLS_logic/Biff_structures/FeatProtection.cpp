#include "stdafx.h"
#include "FeatProtection.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


FeatProtection::FeatProtection()
:	sdContainer_set(false)
{
}


FeatProtection::FeatProtection(CFRecord& record)
:	sdContainer_set(false)
{
	load(record);
}


BiffStructurePtr FeatProtection::clone()
{
	return BiffStructurePtr(new FeatProtection(*this));
}


void FeatProtection::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"fSD", fSD);
	own_tag->setAttribute(L"wPassword", STR::int2hex_str(wPassword, 2).c_str()); // Cut password size since it is really 2-bytes
	own_tag->setAttribute(L"stTitle", stTitle);
	if(sdContainer_set)
	{
		sdContainer.toXML(own_tag);
	}
}


const bool FeatProtection::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### FeatProtection record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FeatProtection record has no BiffStructure::fromXML() implemented.");
	return false;
}


void FeatProtection::store(CFRecord& record)
{
#pragma message("####################### FeatProtection record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FeatProtection record has no BiffStructure::store() implemented.");
	//record << something;
}


void FeatProtection::load(CFRecord& record)
{
	DWORD flags;
	record >> flags;
	fSD = GETBIT(flags, 0);
	record >> wPassword >> stTitle;
	if(!record.isEOF())
	{
		record >> sdContainer;
		sdContainer_set = true;
	}
}


} // namespace XLS

