#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class BookExt_Conditional11 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BookExt_Conditional11)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeBookExt_Conditional11;
	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fBuggedUserAboutSolution;
	bool fShowInkAnnotation;
};

} // namespace XLS

