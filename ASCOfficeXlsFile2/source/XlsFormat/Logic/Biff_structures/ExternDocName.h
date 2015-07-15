#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/ExtNameParsedFormula.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ExternDocName : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExternDocName)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 ixals;
	ShortXLUnicodeString extName;
	ExtNameParsedFormula nameDefinition;
};

} // namespace XLS

