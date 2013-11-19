#pragma once

#include "ParsedFormula.h"

namespace XLS
{;

class CFRecord;
class CellRef;

class ExtNameParsedFormula : public ParsedFormula
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtNameParsedFormula)
public:
	ExtNameParsedFormula();
	BiffStructurePtr clone();
	void load(CFRecord& record);
private:
	// stub to make the class non-abstract
	virtual void store(CFRecord& record);

private:
	PtgPtr val;
};

} // namespace XLS

