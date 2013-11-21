#include "stdafx.h"
#include "XFPropColor.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFPropColor::clone()
{
	return BiffStructurePtr(new XFPropColor(*this));
}


void XFPropColor::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	own_tag->setAttribute(L"fValidRGBA", fValidRGBA);
	own_tag->setAttribute(L"xclrType", xclrType);
	own_tag->setAttribute(L"xclrValue", icv);      //universal names for any color providing structures 
	own_tag->setAttribute(L"numTint", static_cast<double>(nTintShade) / 32767.0); //universal names for any color providing structures
	if(2 == xclrType)
	{
		dwRgba.toXML(own_tag);
	}
}

const bool XFPropColor::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### XFPropColor record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! XFPropColor record has no BiffStructure::fromXML() implemented.");
	return false;
}


void XFPropColor::store(CFRecord& record)
{
#pragma message("####################### XFPropColor record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFPropColor record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFPropColor::load(CFRecord& record)
{
	BYTE flags;
	record >> flags;
	fValidRGBA = GETBIT(flags, 0);
	xclrType = GETBITS(flags, 1, 7);
	record >> icv >> nTintShade >> dwRgba;
}


} // namespace XLS
