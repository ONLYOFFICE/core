#include "stdafx.h"
#include "CFExNonCF12.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr CFExNonCF12::clone()
{
	return BiffStructurePtr(new CFExNonCF12(*this));
}


void CFExNonCF12::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"icf", icf);
	xml_tag->setAttribute(L"cp", cp);
	xml_tag->setAttribute(L"icfTemplate", icfTemplate);
	xml_tag->setAttribute(L"ipriority", ipriority);
	xml_tag->setAttribute(L"fActive", fActive);
	xml_tag->setAttribute(L"fStopIfTrue", fStopIfTrue);
	if(fHasDXF)
	{
		dxf.toXML(xml_tag);
	}
	rgbTemplateParms.toXML(xml_tag, icfTemplate);
}


void CFExNonCF12::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
#pragma message("####################### CFExNonCF12 record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! CFExNonCF12 record has no BiffStructure::fromXML() implemented.");
}


void CFExNonCF12::store(CFRecord& record)
{
#pragma message("####################### CFExNonCF12 record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CFExNonCF12 record has no BiffStructure::store() implemented.");
	//record << something;
}


void CFExNonCF12::load(CFRecord& record)
{
	BYTE flags;
	record >> icf >> cp >> icfTemplate >> ipriority >> flags;
	fActive = GETBIT(flags, 0);
	fStopIfTrue = GETBIT(flags, 1);

	record >> fHasDXF;
	if(fHasDXF)
	{
		record >> dxf;
	}
	BYTE cbTemplateParm;
	record >> cbTemplateParm;
	record >> rgbTemplateParms;
}


} // namespace XLS

