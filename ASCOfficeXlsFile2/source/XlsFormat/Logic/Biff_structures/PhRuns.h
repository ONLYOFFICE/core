#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class PhRuns : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PhRuns)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typePhRuns;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	short ichFirst;
	short ichMom;
	short cchMom;
};

} // namespace XLS

