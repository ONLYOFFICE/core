
#include "MSOCR.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


XLS::BiffStructurePtr MSOCR::clone()
{
	return XLS::BiffStructurePtr(new MSOCR(*this));
}

//
//void MSOCR::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"red", red);
//	xml_tag->setAttribute(L"green", green);
//	xml_tag->setAttribute(L"blue", blue);
//	xml_tag->setAttribute(L"fSchemeIndex", fSchemeIndex);
//}
//
//void MSOCR::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	red = getStructAttribute(xml_tag, L"red");
//	green = getStructAttribute(xml_tag, L"green");
//	blue = getStructAttribute(xml_tag, L"blue");
//	fSchemeIndex = getStructAttribute(xml_tag, L"fSchemeIndex");
//}


void MSOCR::store(XLS::CFRecord& record)
{
	unsigned char flags = 0;
	SETBIT(flags, 3, fSchemeIndex);
	record << red << green << blue << flags;
}


void MSOCR::load(XLS::CFRecord& record)
{
	unsigned char flags;
	record >> red >> green >> blue >> flags;
	fSchemeIndex = GETBIT(flags, 3);
}


} // namespace XLS
