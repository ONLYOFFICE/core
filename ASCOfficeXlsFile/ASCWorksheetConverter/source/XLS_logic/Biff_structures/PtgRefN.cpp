#include "stdafx.h"
#include "PtgRefN.h"
#include "CellRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgRefN::PtgRefN(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgRefN::PtgRefN(const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<BYTE>(data_type) << 5)),
	loc(word),
	cell_base_ref(cell_base_ref_init)
{
	loc -= cell_base_ref;
}


BiffStructurePtr PtgRefN::clone()
{
	return BiffStructurePtr(new PtgRefN(*this));
}


void PtgRefN::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	(loc + cell_base_ref).toXML(xml_tag);
}

void PtgRefN::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgRefN::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgRefN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push((loc + cell_base_ref).toString());
}



} // namespace XLS

