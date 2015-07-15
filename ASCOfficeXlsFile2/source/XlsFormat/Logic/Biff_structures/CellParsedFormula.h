#pragma once

#include "ParsedFormula.h"

namespace XLS
{;

class CFRecord;

class CellParsedFormula : public ParsedFormula
{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(CellParsedFormula)
public:
	CellParsedFormula(const bool is_part_of_a_revision);
	BiffStructurePtr clone();
	void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool is_part_of_a_revision_;
};

} // namespace XLS

