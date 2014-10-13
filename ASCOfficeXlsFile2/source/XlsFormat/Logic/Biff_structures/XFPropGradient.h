#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class XFPropGradient : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFPropGradient)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	Boolean<unsigned int> type;
	double numDegree;
	double numFillToLeft;
	double numFillToRight;
	double numFillToTop;
	double numFillToBottom;
};

} // namespace XLS
