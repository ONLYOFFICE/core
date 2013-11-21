#include "stdafx.h"
#include "PtgRefErr.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgRefErr::PtgRefErr()
{
}


PtgRefErr::PtgRefErr(const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<BYTE>(data_type) << 5))
{
}


BiffStructurePtr PtgRefErr::clone()
{
	return BiffStructurePtr(new PtgRefErr(*this));
}


void PtgRefErr::storeFields(CFRecord& record)
{
	record.reserveNBytes(4); // unused
}


void PtgRefErr::loadFields(CFRecord& record)
{
	record.skipNBytes(4); // unused
}


void PtgRefErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push(L"#REF!");
}


} // namespace XLS

