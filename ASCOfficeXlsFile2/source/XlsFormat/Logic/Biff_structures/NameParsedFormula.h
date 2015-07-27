#pragma once

#include "ParsedFormula.h"

namespace XLS
{;

class CFRecord;

class NameParsedFormula : public ParsedFormula
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(NameParsedFormula)
public:
	// is_part_of_a_revision must be set to "true" from RRDDefName or RRDChgCell records only
	NameParsedFormula(const bool is_part_of_a_revision);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record, const size_t cce);
	virtual void store(CFRecord& record);

	virtual void load(CFRecord& record) {};

	bool is_part_of_a_revision_;
};

} // namespace XLS

