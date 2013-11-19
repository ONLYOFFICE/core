#include "stdafx.h"
#include "PtgAreaN.h"
#include "CellRangeRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgAreaN::PtgAreaN(const CellRef cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgAreaN::PtgAreaN(const std::wstring& word, const PtgDataType data_type, const CellRef cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<BYTE>(data_type) << 5)),
	area(word),
	cell_base_ref(cell_base_ref_init)
{
	area -= cell_base_ref;
}


BiffStructurePtr PtgAreaN::clone()
{
	return BiffStructurePtr(new PtgAreaN(*this));
}


void PtgAreaN::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	(area + cell_base_ref).toXML(xml_tag);
}


void PtgAreaN::storeFields(CFRecord& record)
{
	record << area;
}


void PtgAreaN::loadFields(CFRecord& record)
{
	record >> area;
}


void PtgAreaN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push((area + cell_base_ref).toString());
}


} // namespace XLS

