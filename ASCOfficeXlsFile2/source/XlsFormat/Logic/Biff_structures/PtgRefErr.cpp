
#include "PtgRefErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgRefErr::PtgRefErr()
{
}


PtgRefErr::PtgRefErr(const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
}


BiffStructurePtr PtgRefErr::clone()
{
	return BiffStructurePtr(new PtgRefErr(*this));
}


void PtgRefErr::storeFields(CFRecord& record)
{
	record.reserveNunBytes(4); // unused
}


void PtgRefErr::loadFields(CFRecord& record)
{
	record.skipNunBytes(4); // unused
}


void PtgRefErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(L"#REF!");
}


} // namespace XLS

