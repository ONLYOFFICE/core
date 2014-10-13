#include "precompiled_xls.h"
#include "XFProps.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XFProps::clone()
{
	return BiffStructurePtr(new XFProps(*this));
}

//
//void XFProps::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//	for(size_t i = 0; i < xfPropArray.size(); ++i)
//	{
//		xfPropArray[i].toXML(own_tag);
//	}
//}
//
//const bool XFProps::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### XFProps record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! XFProps record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void XFProps::store(CFRecord& record)
{
#pragma message("####################### XFProps record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFProps record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFProps::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> cprops;
	for(int i = 0; i < cprops; ++i)
	{
		XFProp prop;
		record >> prop;
		xfPropArray.push_back(prop);
	}
}


} // namespace XLS
