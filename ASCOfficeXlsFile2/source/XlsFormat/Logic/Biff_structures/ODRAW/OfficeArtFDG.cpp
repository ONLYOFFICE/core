
#include "OfficeArtFDG.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{

OfficeArtFDG::OfficeArtFDG()
:	OfficeArtRecord(0, FDG)
{
}

XLS::BiffStructurePtr OfficeArtFDG::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFDG(*this));
}


//void OfficeArtFDG::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"csp", csp);
//	xml_tag->setAttribute(L"spidCur", spidCur);
//}
//
//void OfficeArtFDG::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	csp = getStructAttribute(xml_tag, L"csp");
//	spidCur = getStructAttribute(xml_tag, L"spidCur");
//}


void OfficeArtFDG::storeFields(XLS::CFRecord& record)
{
	record << csp << spidCur;
}


void OfficeArtFDG::loadFields(XLS::CFRecord& record)
{
	record >> csp >> spidCur;
}


} // namespace XLS
