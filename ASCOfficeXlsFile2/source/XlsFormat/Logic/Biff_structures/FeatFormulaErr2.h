#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FeatFormulaErr2 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FeatFormulaErr2)
public:
	FeatFormulaErr2();
	FeatFormulaErr2(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeFeatFormulaErr2;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	FFErrorCheck grffecIgnore;
};

} // namespace XLS

