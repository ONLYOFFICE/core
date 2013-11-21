#include "stdafx.h"
#include "PtgRef.h"
#include "CellRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgRef::PtgRef()
{
}


PtgRef::PtgRef(const std::wstring& word, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<BYTE>(data_type) << 5)),
	loc(word)
{
}


BiffStructurePtr PtgRef::clone()
{
	return BiffStructurePtr(new PtgRef(*this));
}


void PtgRef::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	loc.toXML(xml_tag);
}


void PtgRef::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgRef::loadFields(CFRecord& record)
{
	record >> loc;
}

void PtgRef::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push(loc.toString());
}



} // namespace XLS

