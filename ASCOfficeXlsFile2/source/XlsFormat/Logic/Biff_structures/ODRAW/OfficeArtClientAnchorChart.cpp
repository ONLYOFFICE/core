#include "precompiled_xls.h"
#include "OfficeArtClientAnchorChart.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtClientAnchorChart::OfficeArtClientAnchorChart()
:	OfficeArtRecord(0x00, ClientAnchor)
{
}


XLS::BiffStructurePtr OfficeArtClientAnchorChart::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorChart(*this));
}


//void OfficeArtClientAnchorChart::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//// 	xml_tag->setAttribute(L"fMove", fMove);
//	xml_tag->setAttribute(L"fSize", fSize);
//
//	xml_tag->setAttribute(L"lx1", lx1);
//	xml_tag->setAttribute(L"ly1", ly1);
//	xml_tag->setAttribute(L"lx2", lx2);
//	xml_tag->setAttribute(L"ly2", ly2);
//}

//
//void OfficeArtClientAnchorChart::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//// 	fMove = getStructAttribute(xml_tag, L"fMove");
//	fSize = getStructAttribute(xml_tag, L"fSize");
//
//	lx1 = getStructAttribute(xml_tag, L"lx1");
//	ly1 = getStructAttribute(xml_tag, L"ly1");
//	lx2 = getStructAttribute(xml_tag, L"lx2");
//	ly2 = getStructAttribute(xml_tag, L"ly2");
//}


void OfficeArtClientAnchorChart::storeFields(XLS::CFRecord& record)
{
	unsigned __int16 flags = 0;
// 	SETBIT(flags, 0, fMove);
	SETBIT(flags, 1, fSize);
	record << flags << lx1 << ly1 << lx2 << ly2;
}


void OfficeArtClientAnchorChart::loadFields(XLS::CFRecord& record)
{
	unsigned __int16 flags;
	record >> flags >> lx1 >> ly1 >> lx2 >> ly2;
// 	fMove = GETBIT(flags, 0);
	fSize = GETBIT(flags, 1);

}


} // namespace XLS
