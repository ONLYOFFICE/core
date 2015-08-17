#pragma once

#include "ParsedFormula.h"

namespace XLS
{

class CFRecord;

class CFParsedFormula : public ParsedFormula
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFParsedFormula)
public:
	CFParsedFormula(const CellRef& cell_base_ref);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

};

} // namespace XLS

