#include "precompiled_xls.h"
#include "FormatRun.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FormatRun::clone()
{
	return BiffStructurePtr(new FormatRun(*this));
}


//void FormatRun::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ich", ich);
//	ifnt.toXML(xml_tag, "ifnt");
//}
//
//void FormatRun::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	ich = getStructAttribute(xml_tag, L"ich");
//	ifnt.fromXML(xml_tag, "ifnt");
//}


void FormatRun::store(CFRecord& record)
{
	record << ich << ifnt;
}


void FormatRun::load(CFRecord& record)
{
	record >> ich >> ifnt;
	unsigned __int16 _ich = ich;
}


} // namespace XLS
