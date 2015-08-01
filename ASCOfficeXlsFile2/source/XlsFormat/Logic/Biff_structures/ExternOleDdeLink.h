#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/MOper.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ExternOleDdeLink : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExternOleDdeLink)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeExternOleDdeLink;

	unsigned int lStgName;
	ShortXLUnicodeString linkName;
	MOper moper;
};

} // namespace XLS

