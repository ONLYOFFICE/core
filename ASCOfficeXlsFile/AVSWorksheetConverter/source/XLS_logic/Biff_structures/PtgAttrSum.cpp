#include "stdafx.h"
#include "PtgAttrSum.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrSum::clone()
{
	return BiffStructurePtr(new PtgAttrSum(*this));
}


void PtgAttrSum::storeFields(CFRecord& record)
{
	record.reserveNBytes(2); // unused
}


void PtgAttrSum::loadFields(CFRecord& record)
{
	record.skipNBytes(2); // unused
}


void PtgAttrSum::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	addFuncWrapper(ptg_stack, L"SUM");
}

} // namespace XLS

