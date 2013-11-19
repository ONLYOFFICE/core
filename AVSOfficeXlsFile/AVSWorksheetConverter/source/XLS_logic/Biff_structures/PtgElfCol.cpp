#include "stdafx.h"
#include "PtgElfCol.h"
#include "CellRangeRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgElfCol::clone()
{
	return BiffStructurePtr(new PtgElfCol(*this));
}


void PtgElfCol::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	loc.toXML(xml_tag);
}


void PtgElfCol::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgElfCol::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgElfCol::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push(CellRangeRef(loc, CellRangeRef::stColumnBelow).toString());
}



} // namespace XLS

