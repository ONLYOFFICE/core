
#include "SheetId.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr SheetId::clone()
{
	return BiffStructurePtr(new SheetId(*this));
}


//void SheetId::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"id", id);
//}
//
//
//void SheetId::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	id = getStructAttribute(xml_tag, L"id");
//}


void SheetId::store(CFRecord& record)
{
	record << id;
}


void SheetId::load(CFRecord& record)
{
	record >> id;
}


} // namespace XLS

