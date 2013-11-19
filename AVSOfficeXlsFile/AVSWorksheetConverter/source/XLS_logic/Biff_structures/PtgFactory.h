#pragma once

#include "BiffStructure.h"
#include "CellRef.h"
#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgFactory
{
public:
	// Loads a Ptg record
	static PtgPtr loadPtg(CFRecord& record, const CellRef& cell_base_ref);
private:
	static PtgPtr createPtg(const WORD type, const CellRef& cell_base_ref);
};


} // namespace XLS

