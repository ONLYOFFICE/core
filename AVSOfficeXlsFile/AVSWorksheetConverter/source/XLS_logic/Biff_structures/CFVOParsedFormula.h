#pragma once

#include "ParsedFormula.h"

namespace XLS
{;

class CFRecord;

class CFVOParsedFormula : public ParsedFormula
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFVOParsedFormula)
public:
	CFVOParsedFormula();
	BiffStructurePtr clone();
	void load(CFRecord& record);
	virtual void store(CFRecord& record);

};

} // namespace XLS

