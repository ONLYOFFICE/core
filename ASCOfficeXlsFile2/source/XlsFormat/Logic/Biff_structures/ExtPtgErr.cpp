
#include "ExtPtgErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExtPtgErr::clone()
{
	return BiffStructurePtr(new ExtPtgErr(*this));
}

//
//void ExtPtgErr::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"err", err);
//}
//
//
//const bool ExtPtgErr::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExtPtgErr record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExtPtgErr record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void ExtPtgErr::store(CFRecord& record)
{
	record << err;
}


void ExtPtgErr::load(CFRecord& record)
{
	record >> err;
}


void ExtPtgErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(err.toString());
}


} // namespace XLS

