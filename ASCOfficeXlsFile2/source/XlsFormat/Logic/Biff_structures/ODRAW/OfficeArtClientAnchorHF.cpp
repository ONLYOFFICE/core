#include "precompiled_xls.h"
#include "OfficeArtClientAnchorHF.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtClientAnchorHF::OfficeArtClientAnchorHF()
:	OfficeArtRecord(0x00, ClientAnchor)
{
}


XLS::BiffStructurePtr OfficeArtClientAnchorHF::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorHF(*this));
}

//
//void OfficeArtClientAnchorHF::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"width", width);
//	xml_tag->setAttribute(L"height", height);
//}
//
//
//void OfficeArtClientAnchorHF::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	width = getStructAttribute(xml_tag, L"width");
//	height = getStructAttribute(xml_tag, L"height");
//}
//

void OfficeArtClientAnchorHF::storeFields(XLS::CFRecord& record)
{
	record << width << height;
}


void OfficeArtClientAnchorHF::loadFields(XLS::CFRecord& record)
{
	record >> width >> height;
}


} // namespace XLS
