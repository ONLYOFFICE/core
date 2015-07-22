#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/XFPropGradient.h>
#include <Logic/Biff_structures/GradStop.h>

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

	XFPropGradient			gradient;
	unsigned int			cGradStops;
	std::vector<GradStop>	rgGradStops;
};

} // namespace XLS
