#include "stdafx.h"
#include "XFExtNoFRT.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


XFExtNoFRT::XFExtNoFRT()
{
}


XFExtNoFRT::XFExtNoFRT(CFRecord& record)
{
	load(record);
}


BiffStructurePtr XFExtNoFRT::clone()
{
	return BiffStructurePtr(new XFExtNoFRT(*this));
}


void XFExtNoFRT::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	std::for_each(rgExt.begin(), rgExt.end(), boost::bind(&ExtProp::toXML, _1, own_tag));
}


const bool XFExtNoFRT::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### XFExtNoFRT record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! XFExtNoFRT record has no BiffStructure::fromXML() implemented.");
	return false;
}


void XFExtNoFRT::store(CFRecord& record)
{
#pragma message("####################### XFExtNoFRT record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFExtNoFRT record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFExtNoFRT::load(CFRecord& record)
{
	record.skipNBytes(6); // reserved
	WORD cexts;
	record >> cexts;
	for(WORD i = 0; i < cexts; ++i)
	{
		ExtProp prop;
		record >> prop;
		rgExt.push_back(prop);
	}
}


} // namespace XLS

