#include "precompiled_xls.h"
#include "DXFPat.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DXFPat::clone()
{
	return BiffStructurePtr(new DXFPat(*this));
}


//void DXFPat::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"fls", fls);
//	xml_tag->setAttribute(L"icvForeground", icvForeground);
//	xml_tag->setAttribute(L"icvBackground", icvBackground);
//}
//
//
//void DXFPat::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	fls = getStructAttribute(xml_tag, L"fls");
//	icvForeground = getStructAttribute(xml_tag, L"icvForeground");
//	icvBackground = getStructAttribute(xml_tag, L"icvBackground");
//}


void DXFPat::store(CFRecord& record)
{
	unsigned short flags = 0;
	SETBITS(flags, 10, 15, fls);
	record << flags;
	unsigned short flags2 = 0;
	SETBITS(flags2, 0, 6, icvForeground);
	SETBITS(flags2, 7, 13, icvBackground);
	record << flags2;
}


void DXFPat::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fls = GETBITS(flags, 10, 15);
	record >> flags;
	icvForeground = GETBITS(flags, 0, 6);
	icvBackground = GETBITS(flags, 7, 13);
}


} // namespace XLS

