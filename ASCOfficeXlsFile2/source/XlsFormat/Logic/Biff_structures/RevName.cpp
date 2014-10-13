#include "precompiled_xls.h"
#include "RevName.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


RevName::RevName()
{
}


RevName::RevName(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevName::clone()
{
	return BiffStructurePtr(new RevName(*this));
}


//void RevName::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"book", book);
//	if(fExtern)
//	{
//		externName.toXML(xml_tag);
//	}
//	else
//	{
//		name.toXML(xml_tag);
//	}
//}


void RevName::store(CFRecord& record)
{
#pragma message("####################### RevName record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RevName record has no BiffStructure::store() implemented.");
	//record << something;
}


void RevName::load(CFRecord& record)
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
	record >> fExtern;
	if(fExtern)
	{
		record >> externName;
	}
	else
	{
		record >> name;
	}
}


} // namespace XLS

