#include "stdafx.h"
#include "PtgAttrSpaceSemi.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrSpaceSemi::clone()
{
	return BiffStructurePtr(new PtgAttrSpaceSemi(*this));
}


void PtgAttrSpaceSemi::storeFields(CFRecord& record)
{
	record.reserveNBytes(2); // I do not use it, but the data present
}


void PtgAttrSpaceSemi::loadFields(CFRecord& record)
{
	record.skipNBytes(2); // I do not use it, but the data present
}


void PtgAttrSpaceSemi::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	// Display token that shall be skipped
}


} // namespace XLS

