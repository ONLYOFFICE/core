#include "stdafx.h"
#include "PtgElfColS.h"
#include "PtgExtraElf.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgElfColS::clone()
{
	return BiffStructurePtr(new PtgElfColS(*this));
}


void PtgElfColS::storeFields(CFRecord& record)
{
	record.reserveNBytes(4); // unused
}


void PtgElfColS::loadFields(CFRecord& record)
{
	record.skipNBytes(4); // unused
}


void PtgElfColS::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	PtgExtraElfPtr range;
	Log::message("PtgElfColS found!!! PtgElfColS found!!! PtgElfColS found!!! PtgElfColS found!!!");
	if(extra_data.empty() && !(range = boost::dynamic_pointer_cast<PtgExtraElf>(extra_data.front())))
	{
		Log::error("PtgExtraElf must follow the corresponding PtgElfColS.");
		ptg_stack.push(L"#REF!");
		return;
	}
	ptg_stack.push(range->toColumnsSet());
	extra_data.pop();
}

} // namespace XLS

