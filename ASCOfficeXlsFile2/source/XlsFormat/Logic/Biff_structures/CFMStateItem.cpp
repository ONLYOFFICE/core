#include "precompiled_xls.h"
#include "CFMStateItem.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr CFMStateItem::clone()
{
	return BiffStructurePtr(new CFMStateItem(*this));
}

//
//void CFMStateItem::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	cfvo.toXML(xml_tag);
//	xml_tag->setAttribute(L"fEqual", fEqual);
//}
//
//
//void CFMStateItem::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	cfvo.fromXML(xml_tag);
//	fEqual.fromXML(xml_tag, L"fEqual");
//}


void CFMStateItem::store(CFRecord& record)
{
	cfvo.store(record);
	record << fEqual;
	record.reserveNunBytes(4); // unused
}


void CFMStateItem::load(CFRecord& record)
{
	cfvo.load(record);
	record >> fEqual;
	record.skipNunBytes(4); // unused
}


} // namespace XLS

