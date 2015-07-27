
#include "PtgElfRwV.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgElfRwV::clone()
{
	return BiffStructurePtr(new PtgElfRwV(*this));
}

//
//void PtgElfRwV::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	loc.toXML(xml_tag);
//}


void PtgElfRwV::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgElfRwV::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgElfRwV::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(CellRangeRef(loc, CellRangeRef::stRow).toString());
}



} // namespace XLS

