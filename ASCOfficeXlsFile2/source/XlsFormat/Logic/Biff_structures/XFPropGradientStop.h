#pragma once

#include "BiffStructure.h"
#include "XFPropColor.h"

namespace XLS
{;

class CFRecord;

class XFPropGradientStop : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFPropGradientStop)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	double numPosition;
	XFPropColor color;
};

} // namespace XLS
