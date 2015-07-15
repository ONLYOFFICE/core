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

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ShortXLUnicodeString linkName;
};

} // namespace XLS

