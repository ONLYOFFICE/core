#pragma once

#include "ParsedFormula.h"

namespace XLS
{;

class CFRecord;

class DVParsedFormula : public ParsedFormula
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DVParsedFormula)
public:
	DVParsedFormula();
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

};

} // namespace XLS

