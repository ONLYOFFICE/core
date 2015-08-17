
#include "RevExtern.h"
#include <Binary/CFRecord.h>

namespace XLS
{


RevExtern::RevExtern()
{
}


RevExtern::RevExtern(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevExtern::clone()
{
	return BiffStructurePtr(new RevExtern(*this));
}


//void RevExtern::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"book", book);
//	itabFirst.toXML(xml_tag);
//	itabLast.toXML(xml_tag);
//}


void RevExtern::store(CFRecord& record)
{
#pragma message("####################### RevExtern record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RevExtern record has no BiffStructure::store() implemented.");
	//record << something;
}


void RevExtern::load(CFRecord& record)
{
	unsigned char book_type = *record.getCurData<unsigned char>();
	if(0x01 == book_type)
	{
		record.skipNunBytes(2); // current worksheet chosen
	}
	else
	{
		record >> book;
	}
	record >> itabFirst >> itabLast;
}


} // namespace XLS

