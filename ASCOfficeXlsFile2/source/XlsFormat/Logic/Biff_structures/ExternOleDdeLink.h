#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/MOper.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ExternOleDdeLink : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExternOleDdeLink)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned int lStgName;
	ShortXLUnicodeString linkName;
	MOper moper;
};

} // namespace XLS

