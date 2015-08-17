#pragma once

#include "BiffStructure.h"
#include "CellRangeRef.h"

namespace XLS
{;

class CFRecord;

class SqRefU : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SqRefU)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeSqRefU;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const CellRef getLocationFirstCell() const;

	std::wstring  sqref;
};

} // namespace XLS

