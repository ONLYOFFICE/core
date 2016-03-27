
#include "PtgRefN.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgRefN::PtgRefN(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgRefN::PtgRefN(const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	loc(word),
	cell_base_ref(cell_base_ref_init)
{
	loc -= cell_base_ref;
}


BiffStructurePtr PtgRefN::clone()
{
	return BiffStructurePtr(new PtgRefN(*this));
}

void PtgRefN::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgRefN::loadFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char	col;
		_UINT16			rw;
		record >> rw >> col;

		loc.rowRelative	= rw & 0x8000;
		loc.colRelative	= rw & 0x4000;

		loc.column	= col;
		loc.row		= GETBITS(rw, 0, 13);
	}
	else
		record >> loc;
}


void PtgRefN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	ptg_stack.push((loc + cell_base_ref).toString());
}



} // namespace XLS

