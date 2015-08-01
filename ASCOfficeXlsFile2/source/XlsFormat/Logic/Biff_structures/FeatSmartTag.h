#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/FactoidData.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class FeatSmartTag : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FeatSmartTag)
public:
	FeatSmartTag();
	FeatSmartTag(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeFeatSmartTag;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned int hashValue;
	std::vector<FactoidData> rgFactoid;
};

} // namespace XLS

