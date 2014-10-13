#include "precompiled_xls.h"
#include "PtgAreaErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgAreaErr::PtgAreaErr()
{
}


PtgAreaErr::PtgAreaErr(const PtgDataType data_type_init)
:	data_type(data_type_init)
{
}


BiffStructurePtr PtgAreaErr::clone()
{
	return BiffStructurePtr(new PtgAreaErr(*this));
}


void PtgAreaErr::storeFields(CFRecord& record)
{
	record.reserveNunBytes(8); // unused
}


void PtgAreaErr::loadFields(CFRecord& record)
{
	record.skipNunBytes(8); // unused
}


void PtgAreaErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	if(!ptg_stack.empty() && 0 == ptg_stack.top().find(L"#PtgElfRadical"))
	{
		ptg_stack.pop();
	}
	ptg_stack.push(L"#REF!");
}



} // namespace XLS

