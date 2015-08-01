#pragma once

#include "BiffStructure.h"
#include "ObjFmla.h"

namespace XLS
{;

class CFRecord;

class FtMacro : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtMacro)
public:
	FtMacro();
	BiffStructurePtr clone();

	static const ElementType	type = typeFtMacro;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	ObjFmla fmla;
};

} // namespace XLS

