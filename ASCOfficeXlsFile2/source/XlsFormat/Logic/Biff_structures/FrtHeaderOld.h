#pragma once

#include "BiffStructure.h"
#include "../../Binary/CFRecordType.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class FrtHeaderOld : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FrtHeaderOld)
public:
	FrtHeaderOld();
	FrtHeaderOld(const CFRecordType::TypeId type);

	BiffStructurePtr clone();

	static const ElementType	type = typeFrtHeaderOld;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
};

} // namespace XLS

