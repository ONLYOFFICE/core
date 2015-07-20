
#include "HorzBrk.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr HorzBrk::clone()
{
	return BiffStructurePtr(new HorzBrk(*this));
}


//void HorzBrk::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"row", row);
//	xml_tag->setAttribute(L"colStart", colStart);
//	xml_tag->setAttribute(L"colEnd", colEnd);
//}
//
//
//void HorzBrk::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	row = getStructAttribute(xml_tag, L"row");
//	colStart = getStructAttribute(xml_tag, L"colStart");
//	colEnd = getStructAttribute(xml_tag, L"colEnd");
//}


void HorzBrk::store(CFRecord& record)
{
	record << row << colStart << colEnd;
}


void HorzBrk::load(CFRecord& record)
{
	record >> row >> colStart >> colEnd;
}


} // namespace XLS

