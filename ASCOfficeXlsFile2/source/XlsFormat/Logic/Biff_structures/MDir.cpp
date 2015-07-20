
#include "MDir.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr MDir::clone()
{
	return BiffStructurePtr(new MDir(*this));
}

//
//void MDir::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//	own_tag->setAttribute(L"imdt", imdt);
//	own_tag->setAttribute(L"mdd", mdd);
//}
//
//const bool MDir::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### MDir record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! MDir record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void MDir::store(CFRecord& record)
{
	record << imdt << mdd;
}


void MDir::load(CFRecord& record)
{
	record >> imdt >> mdd;
}


} // namespace XLS
