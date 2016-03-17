
#include "PtgExp.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgExp::PtgExp()
:	Ptg(fixed_id)
{

}


BiffStructurePtr PtgExp::clone()
{
	return BiffStructurePtr(new PtgExp(*this));
}


void PtgExp::storeFields(CFRecord& record)
{
	record << row << col;
}


void PtgExp::loadFields(CFRecord& record)
{
	record >> row >> col;
}


void PtgExp::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	// The reference coordinates shall be obtained from row/column values.
	// No textual form need but the empty line.
	ptg_stack.push(L""); 
}


} // namespace XLS

