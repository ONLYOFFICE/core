#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Binary/CFRecordType.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FrtRefHeaderU : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FrtRefHeaderU)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFrtRefHeaderU;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
	Ref8U ref8;
};

} // namespace XLS

