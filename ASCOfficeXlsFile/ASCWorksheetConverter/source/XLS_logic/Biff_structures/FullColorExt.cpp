#include "stdafx.h"
#include "FullColorExt.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FullColorExt::clone()
{
	return BiffStructurePtr(new FullColorExt(*this));
}


void FullColorExt::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	own_tag->setAttribute(L"xclrType", xclrType);
	own_tag->setAttribute(L"numTint", static_cast<double>(nTintShade) / 32767.0); // universal names for any color providing structures
	own_tag->setAttribute(L"xclrValue", xclrValue);
	if(2 == xclrType) // if the type is ARGB
	{
		own_tag->setAttribute(L"argb", STR::toARGB(xclrValue).c_str());
	}
}

const bool FullColorExt::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### FullColorExt record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FullColorExt record has no BiffStructure::fromXML() implemented.");
	return false;
}


void FullColorExt::store(CFRecord& record)
{
#pragma message("####################### FullColorExt record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FullColorExt record has no BiffStructure::store() implemented.");
	//record << something;
}


void FullColorExt::load(CFRecord& record)
{
	record >> xclrType >> nTintShade >> xclrValue;
	record.skipNBytes(8); //unused
}


} // namespace XLS
