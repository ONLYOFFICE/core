#include "precompiled_xls.h"
#include "OfficeArtSplitMenuColorContainer.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtSplitMenuColorContainer::OfficeArtSplitMenuColorContainer()
:	OfficeArtRecord(0, SplitMenuColorContainer)
{
}


XLS::BiffStructurePtr OfficeArtSplitMenuColorContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtSplitMenuColorContainer(*this));
}

//
//void OfficeArtSplitMenuColorContainer::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(smca.begin(), smca.end(), boost::bind(&MSOCR::toXML, _1, xml_tag));
//}
//
//
//void OfficeArtSplitMenuColorContainer::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	MSOCR color;
//	size_t number = 0;
//	while(color.fromXML(xml_tag, ++number, false))
//	{
//		smca.push_back(color);
//	}
//}


void OfficeArtSplitMenuColorContainer::storeFields(XLS::CFRecord& record)
{
	record << smca;
}


void OfficeArtSplitMenuColorContainer::loadFields(XLS::CFRecord& record)
{
	for(size_t i = 0; i < rh_own.recInstance; ++i)
	{
		MSOCR color;
		record >> color;
		smca.push_back(color);
	}
}

const unsigned short OfficeArtSplitMenuColorContainer::GetInstanceToStore()
{
	return smca.size();
}

} // namespace XLS
