
#include "RevNamePly.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


RevNamePly::RevNamePly()
{
}


RevNamePly::RevNamePly(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevNamePly::clone()
{
	return BiffStructurePtr(new RevNamePly(*this));
}

//
//void RevNamePly::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"sheet", sheet);
//	name.toXML(own_tag);
//}
//
//
//const bool RevNamePly::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### RevNamePly record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! RevNamePly record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void RevNamePly::store(CFRecord& record)
{
	record << sheet << name;
}


void RevNamePly::load(CFRecord& record)
{
	record >> sheet >> name;
}


} // namespace XLS

