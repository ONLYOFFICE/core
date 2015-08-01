#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class XFPropGradient : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFPropGradient)
public:
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeXFPropGradient;

	Boolean<unsigned int> type1;
	double numDegree;
	double numFillToLeft;
	double numFillToRight;
	double numFillToTop;
	double numFillToBottom;
};

} // namespace XLS
