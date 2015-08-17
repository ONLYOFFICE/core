#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/CFColor.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class SheetExtOptional : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SheetExtOptional)
public:
	SheetExtOptional();
	SheetExtOptional(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeSheetExtOptional;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char icvPlain12;
	bool fCondFmtCalc;
	bool fNotPublished;
	CFColor color;
};

} // namespace XLS

