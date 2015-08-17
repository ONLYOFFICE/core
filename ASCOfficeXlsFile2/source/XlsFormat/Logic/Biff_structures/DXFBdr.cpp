
#include "DXFBdr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr DXFBdr::clone()
{
	return BiffStructurePtr(new DXFBdr(*this));
}


//void DXFBdr::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"dgLeft", dgLeft);
//	xml_tag->setAttribute(L"dgRight", dgRight);
//	xml_tag->setAttribute(L"dgTop", dgTop);
//	xml_tag->setAttribute(L"dgBottom", dgBottom);
//	xml_tag->setAttribute(L"icvLeft", icvLeft);
//	xml_tag->setAttribute(L"icvRight", icvRight);
//	xml_tag->setAttribute(L"bitDiagDown", bitDiagDown);
//	xml_tag->setAttribute(L"bitDiagUp", bitDiagUp);
//
//	xml_tag->setAttribute(L"icvTop", icvTop);
//	xml_tag->setAttribute(L"icvBottom", icvBottom);
//	xml_tag->setAttribute(L"icvDiag", icvDiag);
//	xml_tag->setAttribute(L"dgDiag", dgDiag);
//}
//
//
//void DXFBdr::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	dgLeft = getStructAttribute(xml_tag, L"dgLeft");
//	dgRight = getStructAttribute(xml_tag, L"dgRight");
//	dgTop = getStructAttribute(xml_tag, L"dgTop");
//	dgBottom = getStructAttribute(xml_tag, L"dgBottom");
//	icvLeft = getStructAttribute(xml_tag, L"icvLeft");
//	icvRight = getStructAttribute(xml_tag, L"icvRight");
//	bitDiagDown = getStructAttribute(xml_tag, L"bitDiagDown");
//	bitDiagUp = getStructAttribute(xml_tag, L"bitDiagUp");
//
//	icvTop = getStructAttribute(xml_tag, L"icvTop");
//	icvBottom = getStructAttribute(xml_tag, L"icvBottom");
//	icvDiag = getStructAttribute(xml_tag, L"icvDiag");
//	dgDiag = getStructAttribute(xml_tag, L"dgDiag");
//}
//
//
void DXFBdr::store(CFRecord& record)
{
	unsigned int flags = 0;
	SETBITS(flags, 0, 3, dgLeft);
	SETBITS(flags, 4, 7, dgRight);
	SETBITS(flags, 8, 11, dgTop);
	SETBITS(flags, 12, 15, dgBottom);
	SETBITS(flags, 16, 22, icvLeft);
	SETBITS(flags, 23, 29, icvRight);
	SETBIT(flags, 30, bitDiagDown);
	SETBIT(flags, 31, bitDiagUp);
	record << flags;

	unsigned int flags2 = 0;
	SETBITS(flags2, 0, 6, icvTop);
	SETBITS(flags2, 7, 13, icvBottom);
	SETBITS(flags2, 14, 20, icvDiag);
	SETBITS(flags2, 21, 24, dgDiag);
	record << flags2;
}


void DXFBdr::load(CFRecord& record)
{
	unsigned int flags;
	record >> flags;
	dgLeft = static_cast<unsigned char>(GETBITS(flags, 0, 3));
	dgRight = static_cast<unsigned char>(GETBITS(flags, 4, 7));
	dgTop = static_cast<unsigned char>(GETBITS(flags, 8, 11));
	dgBottom = static_cast<unsigned char>(GETBITS(flags, 12, 15));
	icvLeft = static_cast<unsigned char>(GETBITS(flags, 16, 22));
	icvRight = static_cast<unsigned char>(GETBITS(flags, 23, 29));
	bitDiagDown = GETBIT(flags, 30);
	bitDiagUp = GETBIT(flags, 31);

	record >> flags;
	icvTop = static_cast<unsigned char>(GETBITS(flags, 0, 6));
	icvBottom = static_cast<unsigned char>(GETBITS(flags, 7, 13));
	icvDiag = static_cast<unsigned char>(GETBITS(flags, 14, 20));
	dgDiag = static_cast<unsigned char>(GETBITS(flags, 21, 24));

}


} // namespace XLS

