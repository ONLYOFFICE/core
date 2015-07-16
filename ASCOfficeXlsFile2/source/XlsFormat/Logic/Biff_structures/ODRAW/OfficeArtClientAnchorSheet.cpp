#include "precompiled_xls.h"
#include "OfficeArtClientAnchorSheet.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtClientAnchorSheet::OfficeArtClientAnchorSheet()
:	OfficeArtRecord(0x00, ClientAnchor)
{
}


XLS::BiffStructurePtr OfficeArtClientAnchorSheet::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorSheet(*this));
}


//void OfficeArtClientAnchorSheet::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"fMove", fMove);
//	xml_tag->setAttribute(L"fSize", fSize);
//
//	xml_tag->setAttribute(L"colL", colL);
//	xml_tag->setAttribute(L"dxL", _dxL);
//	xml_tag->setAttribute(L"rwT", rwT);
//	xml_tag->setAttribute(L"dyT", _dyT);
//	xml_tag->setAttribute(L"colR", colR);
//	xml_tag->setAttribute(L"dxR", _dxR);
//	xml_tag->setAttribute(L"rwB", rwB);
//	xml_tag->setAttribute(L"dyB", _dyB);
//}


//void OfficeArtClientAnchorSheet::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	fMove = getStructAttribute(xml_tag, L"fMove");
//	fSize = getStructAttribute(xml_tag, L"fSize");
//
//	colL = getStructAttribute(xml_tag, L"colL");
//	dxL = getStructAttribute(xml_tag, L"dxL");
//	rwT = getStructAttribute(xml_tag, L"rwT");
//	dyT = getStructAttribute(xml_tag, L"dyT");
//	colR = getStructAttribute(xml_tag, L"colR");
//	dxR = getStructAttribute(xml_tag, L"dxR");
//	rwB = getStructAttribute(xml_tag, L"rwB");
//	dyB = getStructAttribute(xml_tag, L"dyB");
//}


void OfficeArtClientAnchorSheet::storeFields(XLS::CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fMove);
	SETBIT(flags, 1, fSize);
	record << flags << colL << dxL << rwT << dyT << colR << dxR << rwB << dyB;
}


void OfficeArtClientAnchorSheet::loadFields(XLS::CFRecord& record)
{
	unsigned short flags;
	record >> flags >> colL >> dxL >> rwT >> dyT >> colR >> dxR >> rwB >> dyB;
	fMove = GETBIT(flags, 0);
	fSize = GETBIT(flags, 1);

	_dxL = dxL * 595;
	_dyT = dyT * 744;
	_dxR = dxR * 595;
	_dyB = dyB * 744;
}


} // namespace XLS
