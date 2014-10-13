#include "precompiled_xls.h"
#include "RRD.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


RRD::RRD()
{
}


RRD::RRD(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RRD::clone()
{
	return BiffStructurePtr(new RRD(*this));
}

//
//void RRD::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"cbMemory", cbMemory);
//	own_tag->setAttribute(L"revid", revid);
//	own_tag->setAttribute(L"revt", revt.type);
//	own_tag->setAttribute(L"fUndoAction", fUndoAction);
//	own_tag->setAttribute(L"fDelAtEdgeOfSort", fDelAtEdgeOfSort);
//	own_tag->setAttribute(L"tabid", tabid);
//}
//
//
//const bool RRD::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### RRD record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! RRD record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void RRD::store(CFRecord& record)
{
#pragma message("####################### RRD record has no BiffStructure::store() implemented")
	Log::error(" Error!!! RRD record has no BiffStructure::store() implemented.");
	//record << something;
}


void RRD::load(CFRecord& record)
{
	unsigned __int16 flags;
	record >> cbMemory >> revid >> revt >> flags;
	fAccepted = GETBIT(flags, 0);
	fUndoAction = GETBIT(flags, 1);
	fDelAtEdgeOfSort = GETBIT(flags, 3);
	record >> tabid;
}


} // namespace XLS

