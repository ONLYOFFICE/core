
#include "ISSTInf.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ISSTInf::clone()
{
	return BiffStructurePtr(new ISSTInf(*this));
}


//void ISSTInf::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ib", ib);
//	xml_tag->setAttribute(L"cbOffset", cbOffset);
//}
//
//void ISSTInf::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	ib = getStructAttribute(xml_tag, L"ib");
//	cbOffset = getStructAttribute(xml_tag, L"cbOffset");
//}


void ISSTInf::store(CFRecord& record)
{
	record << ib << cbOffset;
	record.reserveNunBytes(2); // reserved
}


void ISSTInf::load(CFRecord& record)
{
	record >> ib >> cbOffset;
	record.skipNunBytes(2); // reserved
}


} // namespace XLS

