#include "precompiled_xls.h"
#include "CFVO.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr CFVO::clone()
{
	return BiffStructurePtr(new CFVO(*this));
}

//
//void CFVO::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"cfvoType", cfvoType);
//	fmla.toXML(xml_tag);
//	if(!fmla.getCCE())
//	{
//		xml_tag->setAttribute(L"numValue", numValue);
//	}
//}
//
//
//void CFVO::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	cfvoType = getStructAttribute(xml_tag, L"cfvoType");
//	if(!fmla.fromXML(xml_tag))
//	{
//		numValue = getStructAttribute(xml_tag, L"numValue");
//	}
//}


void CFVO::store(CFRecord& record)
{
	record << cfvoType;
	fmla.store(record);
	if(!fmla.getCCE())
	{
		record << numValue;
	}
}


void CFVO::load(CFRecord& record)
{
	record >> cfvoType;
	fmla.load(record);
	if(!fmla.getCCE())
	{
		record >> numValue;
	}
}


} // namespace XLS

