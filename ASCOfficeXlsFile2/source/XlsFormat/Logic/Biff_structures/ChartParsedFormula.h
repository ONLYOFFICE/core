#pragma once

#include "ParsedFormula.h"
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{

class CFRecord;

class ChartParsedFormula : public ParsedFormula
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ChartParsedFormula)
public:
	ChartParsedFormula();

	BiffStructurePtr clone();
	void load(CFRecord& record);
	
	virtual void store(CFRecord& record);

	std::vector<CellRangeRef>	cell_ranges;

	bool inRange(const CellRef & ref);
};

} // namespace XLS

