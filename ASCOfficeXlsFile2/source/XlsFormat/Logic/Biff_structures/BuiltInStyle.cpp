#include "precompiled_xls.h"
#include "BuiltInStyle.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr BuiltInStyle::clone()
{
	return BiffStructurePtr(new BuiltInStyle(*this));
}


//void BuiltInStyle::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"istyBuiltIn", istyBuiltIn);
//// 	if(255 != iLevel)
//// 	{
//		xml_tag->setAttribute(L"iLevel", iLevel);
//// 	}
//}
//
//void BuiltInStyle::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	istyBuiltIn = getStructAttribute(xml_tag, L"istyBuiltIn");
//// 	if(255 != iLevel)
//// 	{
//		iLevel = getStructAttribute(xml_tag, L"iLevel");
//// 	}
//}


void BuiltInStyle::store(CFRecord& record)
{
	record << istyBuiltIn << iLevel;
}


void BuiltInStyle::load(CFRecord& record)
{
	record >> istyBuiltIn >> iLevel;
}


} // namespace XLS
