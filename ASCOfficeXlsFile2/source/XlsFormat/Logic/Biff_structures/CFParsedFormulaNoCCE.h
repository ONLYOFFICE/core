#pragma once

#include "ParsedFormula.h"

namespace XLS
{;

class CFRecord;

class CFParsedFormulaNoCCE : public ParsedFormula
{
		BASE_STRUCTURE_DEFINE_CLASS_NAME(CFParsedFormulaNoCCE)
public:
	CFParsedFormulaNoCCE(const CellRef& cell_base_ref_init);
	BiffStructurePtr clone();
	void load(CFRecord& record, const unsigned __int16 cce);
	virtual void store(CFRecord& record);
private:
	// stub to make the class non-abstract
	virtual void load(CFRecord& record);


};

} // namespace XLS

