
#include "DXFALC.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr DXFALC::clone()
{
	return BiffStructurePtr(new DXFALC(*this));
}


//void DXFALC::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"alc", alc);
//	xml_tag->setAttribute(L"fWrap", fWrap);
//	xml_tag->setAttribute(L"alcv", alcv);
//	xml_tag->setAttribute(L"fJustLast", fJustLast);
//	xml_tag->setAttribute(L"trot", trot);
//	xml_tag->setAttribute(L"cIndent", cIndent);
//	xml_tag->setAttribute(L"fShrinkToFit", fShrinkToFit);
//	xml_tag->setAttribute(L"fMergeCell", fMergeCell);
//	xml_tag->setAttribute(L"iReadingOrder", iReadingOrder);
//	
//	xml_tag->setAttribute(L"iIndent", iIndent);
//}
//
//
//void DXFALC::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	alc = getStructAttribute(xml_tag, L"alc");
//	fWrap = getStructAttribute(xml_tag, L"fWrap");
//	alcv = getStructAttribute(xml_tag, L"alcv");
//	fJustLast = getStructAttribute(xml_tag, L"fJustLast");
//	trot = getStructAttribute(xml_tag, L"trot");
//	cIndent = getStructAttribute(xml_tag, L"cIndent");
//	fShrinkToFit = getStructAttribute(xml_tag, L"fShrinkToFit");
//	fMergeCell = getStructAttribute(xml_tag, L"fMergeCell");
//	iReadingOrder = getStructAttribute(xml_tag, L"iReadingOrder");
//
//	iIndent = getStructAttribute(xml_tag, L"iIndent");
//}
//
//
void DXFALC::store(CFRecord& record)
{
	unsigned int flags = 0;
	SETBITS(flags, 0, 2, alc);
	SETBIT(flags, 3, fWrap);
	SETBITS(flags, 4, 6, alcv);
	SETBIT(flags, 7, fJustLast);
	SETBITS(flags, 8, 15, trot);
	SETBITS(flags, 16, 19, cIndent);
	SETBIT(flags, 20, fShrinkToFit);
	SETBIT(flags, 21, fMergeCell);
	SETBITS(flags, 22, 23, iReadingOrder);
	record << flags;
	record << iIndent;
}


void DXFALC::load(CFRecord& record)
{
	unsigned int flags;
	record >> flags;
	alc = static_cast<unsigned char>(GETBITS(flags, 0, 2));
	fWrap = GETBIT(flags, 3);
	alcv = static_cast<unsigned char>(GETBITS(flags, 4, 6));
	fJustLast = GETBIT(flags, 7);
	trot = static_cast<unsigned char>(GETBITS(flags, 8, 15));
	cIndent = static_cast<unsigned char>(GETBITS(flags, 16, 19));
	fShrinkToFit = GETBIT(flags, 20);
	fMergeCell = GETBIT(flags, 21);
	iReadingOrder = static_cast<unsigned char>(GETBITS(flags, 22, 23));

	record >> iIndent;
}


} // namespace XLS

