
#include "OfficeArtFDGGBlock.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtFDGGBlock::OfficeArtFDGGBlock()
:	OfficeArtRecord(0, FDGGBlock)
{
}


XLS::BiffStructurePtr OfficeArtFDGGBlock::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFDGGBlock(*this));
}

//
//void OfficeArtFDGGBlock::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"spidMax", spidMax);
//	xml_tag->setAttribute(L"cspSaved", cspSaved);
//	xml_tag->setAttribute(L"cdgSaved", cdgSaved);
//	std::for_each(Rgidcl.begin(), Rgidcl.end(), boost::bind(&OfficeArtIDCL::toXML, _1, xml_tag));
//}

//
//void OfficeArtFDGGBlock::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	spidMax = getStructAttribute(xml_tag, L"spidMax");
//	cspSaved = getStructAttribute(xml_tag, L"cspSaved");
//	cdgSaved = getStructAttribute(xml_tag, L"cdgSaved");
//	OfficeArtIDCL idcl;
//	size_t number = 0;
//	while(idcl.fromXML(xml_tag, ++number, false))
//	{
//		Rgidcl.push_back(idcl);
//	}
//}


void OfficeArtFDGGBlock::storeFields(XLS::CFRecord& record)
{
	unsigned int cidcl = Rgidcl.size();
	record << spidMax << cidcl << cspSaved << cdgSaved;
	record << Rgidcl;
}


void OfficeArtFDGGBlock::loadFields(XLS::CFRecord& record)
{
	unsigned int cidcl;
	record >> spidMax >> cidcl >> cspSaved >> cdgSaved;

	for(size_t i = 0; i < cidcl - 1; ++i)
	{
		OfficeArtIDCL idcl;
		record >> idcl;
		Rgidcl.push_back(idcl);
	}
}

} // namespace XLS
