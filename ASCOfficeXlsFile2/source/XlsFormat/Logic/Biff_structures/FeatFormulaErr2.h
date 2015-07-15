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

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	FFErrorCheck grffecIgnore;
};

} // namespace XLS

