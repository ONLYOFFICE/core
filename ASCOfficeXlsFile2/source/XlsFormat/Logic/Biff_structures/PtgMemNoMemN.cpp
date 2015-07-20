
#include "PtgMemNoMemN.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgMemNoMemN::clone()
{
	return BiffStructurePtr(new PtgMemNoMemN(*this));
}

//
//void PtgMemNoMemN::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"type", type);
//}
//
//
//const bool PtgMemNoMemN::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### PtgMemNoMemN record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! PtgMemNoMemN record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void PtgMemNoMemN::store(CFRecord& record)
{
#pragma message("####################### PtgMemNoMemN record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgMemNoMemN record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgMemNoMemN::load(CFRecord& record)
{
	unsigned char rec_type;
	record >> rec_type;
	type = GETBITS(rec_type, 5, 6);
	record >> cce;
}


void PtgMemNoMemN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	// I assume it doesn't generate an error.
}




} // namespace XLS

