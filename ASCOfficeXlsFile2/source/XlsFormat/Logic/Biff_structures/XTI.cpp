
#include "XTI.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XTI::clone()
{
	return BiffStructurePtr(new XTI(*this));
}


//void XTI::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"iSupBook", iSupBook);
//	xml_tag->setAttribute(L"itabFirst", itabFirst);
//	xml_tag->setAttribute(L"itabLast", itabLast);
//}
//
//void XTI::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	iSupBook = getStructAttribute(xml_tag, L"iSupBook");
//	itabFirst = getStructAttribute(xml_tag, L"itabFirst");
//	itabLast = getStructAttribute(xml_tag, L"itabLast");
//}


void XTI::store(CFRecord& record)
{
	record << iSupBook << itabFirst << itabLast;
}


void XTI::load(CFRecord& record)
{
	record >> iSupBook >> itabFirst >> itabLast;

	record.getGlobalWorkbookInfo()->xti_parsed.push_back(
		XMLSTUFF::xti_indexes2sheet_name(itabFirst, itabLast, record.getGlobalWorkbookInfo()->sheets_names));
}


} // namespace XLS
