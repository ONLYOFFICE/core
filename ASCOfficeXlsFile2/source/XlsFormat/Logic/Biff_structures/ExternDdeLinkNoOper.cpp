
#include "ExternDdeLinkNoOper.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExternDdeLinkNoOper::clone()
{
	return BiffStructurePtr(new ExternDdeLinkNoOper(*this));
}


//void ExternDdeLinkNoOper::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"linkName", linkName);
//}
//
//
//const bool ExternDdeLinkNoOper::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExternDdeLinkNoOper record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExternDdeLinkNoOper record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void ExternDdeLinkNoOper::store(CFRecord& record)
{
#pragma message("####################### ExternDdeLinkNoOper record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternDdeLinkNoOper record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternDdeLinkNoOper::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved
	record >> linkName;
}


} // namespace XLS

