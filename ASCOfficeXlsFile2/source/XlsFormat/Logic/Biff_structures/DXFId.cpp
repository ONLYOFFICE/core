#include "precompiled_xls.h"
#include "DXFId.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


DXFId::DXFId()
{
}


DXFId::DXFId(CFRecord& record)
{
	load(record);
}


BiffStructurePtr DXFId::clone()
{
	return BiffStructurePtr(new DXFId(*this));
}


//void DXFId::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"index", index);
//}
//
//
//const bool DXFId::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### DXFId record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! DXFId record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void DXFId::store(CFRecord& record)
{
	record << index;
}


void DXFId::load(CFRecord& record)
{
	record >> index;
}


} // namespace XLS

