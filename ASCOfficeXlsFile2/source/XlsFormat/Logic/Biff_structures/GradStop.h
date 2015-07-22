#pragma once

#include "BiffStructure.h"
#include "LongRGB.h"

namespace XLS
{;

class CFRecord;

class GradStop : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(GradStop)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short xclrType;
	unsigned int xclrValue;
	LongRGBA xclrValue_rgb;
	double numPosition;
	double numTint;
};

} // namespace XLS
