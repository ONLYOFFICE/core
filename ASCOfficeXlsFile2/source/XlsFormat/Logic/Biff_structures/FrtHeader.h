#pragma once

#include "BiffStructure.h"
#include <Binary/CFRecordType.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FrtHeader : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FrtHeader)
public:
	FrtHeader();
	FrtHeader(const CFRecordType::TypeId type);

	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	
	static const ElementType	type = typeFrtHeader;
	
	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
};

typedef boost::shared_ptr<FrtHeader> FrtHeaderPtr;

} // namespace XLS

