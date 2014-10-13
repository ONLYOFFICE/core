#include "precompiled_xls.h"
#include "PtgElfRw.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgElfRw::clone()
{
	return BiffStructurePtr(new PtgElfRw(*this));
}

//
//void PtgElfRw::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	loc.toXML(xml_tag);
//}


void PtgElfRw::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgElfRw::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgElfRw::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.push(CellRangeRef(loc, CellRangeRef::stRow).toString());
}



} // namespace XLS

