#pragma once

#include "BiffStructure.h"
#include "BiffString.h"
#include "RevLblName.h"

namespace XLS
{;

class CFRecord;

class RevNamePly : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevNamePly)
public:
	RevNamePly();
	RevNamePly(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeRevNamePly;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	RevSheetName sheet;
	RevLblName name;
};

} // namespace XLS

