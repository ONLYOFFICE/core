
#include "ItemMoniker.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


ItemMoniker::ItemMoniker()
{
}


ItemMoniker::ItemMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr ItemMoniker::clone()
{
	return XLS::BiffStructurePtr(new ItemMoniker(*this));
}

//
//void ItemMoniker::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"delimiterAnsi", delimiterAnsi.c_str());
//	xml_tag->setAttribute(L"delimiterUnicode", delimiterUnicode.c_str());
//	xml_tag->setAttribute(L"itemAnsi", delimiterAnsi.c_str());
//	xml_tag->setAttribute(L"itemUnicode", delimiterUnicode.c_str());
//}
//
//
//void ItemMoniker::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//#pragma message("####################### ItemMoniker record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ItemMoniker record has no BiffStructure::fromXML() implemented.");
//}


void ItemMoniker::store(XLS::CFRecord& record)
{
#pragma message("####################### ItemMoniker record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ItemMoniker record has no BiffStructure::store() implemented.");
	//record << something;
}


void ItemMoniker::load(XLS::CFRecord& record)
{
	unsigned int delimiterLength;
	record >> delimiterLength >> delimiterAnsi;
	int sizeof_delimiterUnicode = delimiterLength - (delimiterAnsi.length() + 1);
	if(sizeof_delimiterUnicode > 0)
	{
		delimiterUnicode = std::wstring(record.getCurData<wchar_t>(), sizeof_delimiterUnicode / 2);
		record.skipNunBytes(sizeof_delimiterUnicode);
	}

	unsigned int itemLength;
	record >> itemLength >> itemAnsi;
	int sizeof_itemUnicode = itemLength - (itemAnsi.length() + 1);
	if(sizeof_itemUnicode > 0)
	{
		itemUnicode = std::wstring(record.getCurData<wchar_t>(), sizeof_itemUnicode / 2);
		record.skipNunBytes(sizeof_itemUnicode);
	}

}


} // namespace OSHARED

