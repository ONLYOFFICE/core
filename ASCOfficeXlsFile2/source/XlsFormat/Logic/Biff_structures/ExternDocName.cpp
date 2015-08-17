
#include "ExternDocName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ExternDocName::clone()
{
	return BiffStructurePtr(new ExternDocName(*this));
}


//void ExternDocName::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"ixals", ixals);
//	own_tag->setAttribute(L"extName", extName);
//	nameDefinition.toXML(own_tag);
//}


//const bool ExternDocName::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExternDocName record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExternDocName record has no BiffStructure::fromXML() implemented.");
//	return false;
//}
//

void ExternDocName::store(CFRecord& record)
{
#pragma message("####################### ExternDocName record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternDocName record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternDocName::load(CFRecord& record)
{
	record >> ixals;
	record.skipNunBytes(2); // reserved
	record >> extName;
	nameDefinition.load(record);
}


} // namespace XLS

