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

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 xclrType;
	unsigned int xclrValue;
	LongRGBA xclrValue_rgb;
	double numPosition;
	double numTint;
};

} // namespace XLS
