#pragma once

#include "ParsedFormula.h"

namespace XLS
{

class CFRecord;

class ObjectParsedFormula : public ParsedFormula
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ObjectParsedFormula)
public:
	ObjectParsedFormula();
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

};

} // namespace XLS

