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

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	short ichFirst;
	short ichMom;
	short cchMom;
};

} // namespace XLS

