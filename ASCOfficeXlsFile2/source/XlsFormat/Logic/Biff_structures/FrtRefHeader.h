#pragma once

#include "BiffStructure.h"
#include "CellRangeRef.h"
#include <Binary/CFRecordType.h>
#include "BitMarkedStructs.h"

namespace XLS
{

class CFRecord;

class FrtRefHeader : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FrtRefHeader)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFrtRefHeader;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

//private
public:
	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
	Ref8U ref8;
};

} // namespace XLS

