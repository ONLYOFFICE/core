#include "stdafx.h"
#include "PtgAttrSemi.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrSemi::clone()
{
	return BiffStructurePtr(new PtgAttrSemi(*this));
}


void PtgAttrSemi::storeFields(CFRecord& record)
{
	record.reserveNBytes(2); // unused
}


void PtgAttrSemi::loadFields(CFRecord& record)
{
	record.skipNBytes(2); // unused
}


void PtgAttrSemi::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	// volatile property is not seen in the textual form
}


} // namespace XLS

