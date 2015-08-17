
#include "CompositeMoniker.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


CompositeMoniker::CompositeMoniker()
{
}


CompositeMoniker::CompositeMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr CompositeMoniker::clone()
{
	return XLS::BiffStructurePtr(new CompositeMoniker(*this));
}

//
//void CompositeMoniker::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(monikerArray.begin(), monikerArray.end(), boost::bind(&HyperlinkMoniker::toXML, _1, xml_tag));
//}
//
//
//void CompositeMoniker::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//#pragma message("####################### CompositeMoniker record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! CompositeMoniker record has no BiffStructure::fromXML() implemented.");
//}


void CompositeMoniker::store(XLS::CFRecord& record)
{
#pragma message("####################### CompositeMoniker record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CompositeMoniker record has no BiffStructure::store() implemented.");
	//record << something;
}


void CompositeMoniker::load(XLS::CFRecord& record)
{
	unsigned int cMonikers;

	record >> cMonikers;
	for (size_t i = 0; i < cMonikers ; i++)
	{
		HyperlinkMoniker mon;
		record >> mon;
		monikerArray.push_back(mon);
	}
}


} // namespace OSHARED

