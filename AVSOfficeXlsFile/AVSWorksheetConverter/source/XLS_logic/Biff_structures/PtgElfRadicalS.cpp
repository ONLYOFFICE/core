#include "stdafx.h"
#include "PtgElfRadicalS.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgElfRadicalS::clone()
{
	return BiffStructurePtr(new PtgElfRadicalS(*this));
}


void PtgElfRadicalS::storeFields(CFRecord& record)
{
	record.reserveNBytes(4); // unused
}


void PtgElfRadicalS::loadFields(CFRecord& record)
{
	record.skipNBytes(4); // unused
}


void PtgElfRadicalS::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	// The significant tag follows (PtgArea or PtgAreaErr)
}


} // namespace XLS

