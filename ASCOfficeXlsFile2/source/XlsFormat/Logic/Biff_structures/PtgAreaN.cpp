#include "precompiled_xls.h"
#include "PtgAreaN.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgAreaN::PtgAreaN(const CellRef cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgAreaN::PtgAreaN(const std::wstring& word, const PtgDataType data_type, const CellRef cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	area(word),
	cell_base_ref(cell_base_ref_init)
{
	area -= cell_base_ref;
}


BiffStructurePtr PtgAreaN::clone()
{
	return BiffStructurePtr(new PtgAreaN(*this));
}


//void PtgAreaN::setXMLAttributes(BiffStructurePtr & xml_tag)
//{
//	(area + cell_base_ref).toXML(xml_tag);
//}


void PtgAreaN::storeFields(CFRecord& record)
{
	record << area;
}


void PtgAreaN::loadFields(CFRecord& record)
{
	record >> area;
}


void PtgAreaN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	ptg_stack.push((area + cell_base_ref).toString());
}


} // namespace XLS

