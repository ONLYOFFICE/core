
#include "OfficeArtFRIT.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


XLS::BiffStructurePtr OfficeArtFRIT::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFRIT(*this));
}

//
//void OfficeArtFRIT::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"fridNew", fridNew);
//	xml_tag->setAttribute(L"fridOld", fridOld);
//}
//
//void OfficeArtFRIT::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	fridNew = getStructAttribute(xml_tag, L"fridNew");
//	fridOld = getStructAttribute(xml_tag, L"fridOld");
//}


void OfficeArtFRIT::store(XLS::CFRecord& record)
{
	record << fridNew << fridOld;
}


void OfficeArtFRIT::load(XLS::CFRecord& record)
{
	record >> fridNew >> fridOld;
}


} // namespace XLS
