#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;

class CFRecord;

class SqRef : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SqRef)
public:
	BiffStructurePtr clone();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	std::vector<Ref8U> rgrefs;
};

} // namespace XLS

