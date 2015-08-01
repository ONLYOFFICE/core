#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ExternDdeLinkNoOper : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExternDdeLinkNoOper)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeExternDdeLinkNoOper;

	ShortXLUnicodeString linkName;
};

} // namespace XLS

