
#include "DXFProt.h"
#include <Binary/CFRecord.h>

namespace XLS
{;



BiffStructurePtr DXFProt::clone()
{
	return BiffStructurePtr(new DXFProt(*this));
}


//void DXFProt::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"fLocked", fLocked);
//	xml_tag->setAttribute(L"fHidden", fHidden);
//}
//
//
//void DXFProt::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	fLocked = getStructAttribute(xml_tag, L"fLocked");
//	fHidden = getStructAttribute(xml_tag, L"fHidden");
//}


void DXFProt::store(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fLocked);
	SETBIT(flags, 1, fHidden);
	record << flags;
}


void DXFProt::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fLocked = GETBIT(flags, 0);
	fHidden = GETBIT(flags, 1);
}


} // namespace XLS

