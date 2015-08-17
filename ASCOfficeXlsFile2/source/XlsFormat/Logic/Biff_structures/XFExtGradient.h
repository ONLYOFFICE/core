#pragma once

#include "BiffStructure.h"
#include "XFPropGradient.h"
#include "GradStop.h"

namespace XLS
{;

class CFRecord;

class XFExtGradient : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFExtGradient)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeXFExtGradient;

	XFPropGradient			gradient;
	unsigned int			cGradStops;
	std::vector<GradStop>	rgGradStops;
};

} // namespace XLS
