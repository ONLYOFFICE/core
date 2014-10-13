#include "precompiled_xls.h"
#include "PropertyBagStore.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{;


XLS::BiffStructurePtr PropertyBagStore::clone()
{
	return XLS::BiffStructurePtr(new PropertyBagStore(*this));
}

//
//void PropertyBagStore::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(factoidTypes.begin(), factoidTypes.end(), boost::bind(&FactoidType::toXML, _1, xml_tag));
//	xml_tag->setAttribute(L"cbHdr", cbHdr);
//	xml_tag->setAttribute(L"sVer", sVer);
//	for(std::vector<PBString>::iterator it = stringTable.begin(), itEnd = stringTable.end(); it != itEnd; ++it)
//	{
//		MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(L"PBString", xml_tag);
//		it->toXML(own_tag, L"str");
//	}
//}

//
//void PropertyBagStore::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//#pragma message("####################### PropertyBagStore record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! PropertyBagStore record has no BiffStructure::fromXML() implemented.");
//}
//

void PropertyBagStore::store(XLS::CFRecord& record)
{
#pragma message("####################### PropertyBagStore record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PropertyBagStore record has no BiffStructure::store() implemented.");
	//record << something;
}


void PropertyBagStore::load(XLS::CFRecord& record)
{
	unsigned int cFactoidType;
	record >> cFactoidType;
	for(size_t i = 0; i < cFactoidType; ++i)
	{
		FactoidType f;
		record >> f;
		factoidTypes.push_back(f);
	}
	record >> cbHdr >> sVer;
	record.skipNunBytes(4); // ignored
	unsigned int cste;
	record >> cste;
	for(size_t i = 0; i < cste; ++i)
	{
		PBString f;
		record >> f;
		stringTable.push_back(f);
	}
}


} // namespace OSHARED

