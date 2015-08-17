
#include "VertBrk.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr VertBrk::clone()
{
	return BiffStructurePtr(new VertBrk(*this));
}

//
//void VertBrk::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"col", col);
//	xml_tag->setAttribute(L"rowStart", rowStart);
//	xml_tag->setAttribute(L"rowEnd", rowEnd);
//}
//
//
//void VertBrk::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	col = getStructAttribute(xml_tag, L"col");
//	rowStart = getStructAttribute(xml_tag, L"rowStart");
//	rowEnd = getStructAttribute(xml_tag, L"rowEnd");
//}


void VertBrk::store(CFRecord& record)
{
	record << col << rowStart << rowEnd;
}


void VertBrk::load(CFRecord& record)
{
	record >> col >> rowStart >> rowEnd;
}

} // namespace XLS

