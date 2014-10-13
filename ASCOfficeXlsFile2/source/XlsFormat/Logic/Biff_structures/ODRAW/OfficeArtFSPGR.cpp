#include "precompiled_xls.h"
#include "OfficeArtFSPGR.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtFSPGR::OfficeArtFSPGR()
:	OfficeArtRecord(0x01, FSPGR)
{
}


XLS::BiffStructurePtr OfficeArtFSPGR::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFSPGR(*this));
}


//void OfficeArtFSPGR::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"xLeft", xLeft);
//	xml_tag->setAttribute(L"yTop", yTop);
//	xml_tag->setAttribute(L"xRight", xRight);
//	xml_tag->setAttribute(L"yBottom", yBottom);
//}

//
//void OfficeArtFSPGR::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xLeft = getStructAttribute(xml_tag, L"xLeft");
//	yTop = getStructAttribute(xml_tag, L"yTop");
//	xRight = getStructAttribute(xml_tag, L"xRight");
//	yBottom = getStructAttribute(xml_tag, L"yBottom");
//}


void OfficeArtFSPGR::storeFields(XLS::CFRecord& record)
{
	record << xLeft << yTop << xRight << yBottom;
}


void OfficeArtFSPGR::loadFields(XLS::CFRecord& record)
{
	record >> xLeft >> yTop >> xRight >> yBottom;
}


} // namespace XLS
