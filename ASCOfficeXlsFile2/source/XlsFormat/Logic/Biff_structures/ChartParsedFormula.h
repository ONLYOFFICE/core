#pragma once

#include "ParsedFormula.h"

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

};

} // namespace XLS

