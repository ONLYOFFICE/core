
#include "PropertyBag.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{;


XLS::BiffStructurePtr PropertyBag::clone()
{
	return XLS::BiffStructurePtr(new PropertyBag(*this));
}

//
//void PropertyBag::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"id", id);
//	std::for_each(properties.begin(), properties.end(), boost::bind(&Property::toXML, _1, own_tag));
//}
//
//
//const bool PropertyBag::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### PropertyBag record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! PropertyBag record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void PropertyBag::store(XLS::CFRecord& record)
{
#pragma message("####################### PropertyBag record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PropertyBag record has no BiffStructure::store() implemented.");
	//record << something;
}


void PropertyBag::load(XLS::CFRecord& record)
{
	unsigned short cProp;
	record >> id >> cProp;
	record.skipNunBytes(2); // ignored
	for (int i = 0; i < cProp ; i++)
	{
		Property prop;
		record >> prop;
		properties.push_back(prop);
	}
}


} // namespace OSHARED

