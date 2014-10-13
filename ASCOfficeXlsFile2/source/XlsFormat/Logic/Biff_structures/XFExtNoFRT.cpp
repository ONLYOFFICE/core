#include "precompiled_xls.h"
#include "XFExtNoFRT.h"
#include <Binary/CFRecord.h>

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

//
//void XFExtNoFRT::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	std::for_each(rgExt.begin(), rgExt.end(), boost::bind(&ExtProp::toXML, _1, own_tag));
//}

//
//const bool XFExtNoFRT::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### XFExtNoFRT record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! XFExtNoFRT record has no BiffStructure::fromXML() implemented.");
//	return false;
//}
//

void XFExtNoFRT::store(CFRecord& record)
{
#pragma message("####################### XFExtNoFRT record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFExtNoFRT record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFExtNoFRT::load(CFRecord& record)
{
	record.skipNunBytes(6); // reserved
	unsigned __int16 cexts;
	record >> cexts;
	for(unsigned __int16 i = 0; i < cexts; ++i)
	{
		ExtProp prop;
		record >> prop;
		rgExt.push_back(prop);
	}
}


} // namespace XLS

