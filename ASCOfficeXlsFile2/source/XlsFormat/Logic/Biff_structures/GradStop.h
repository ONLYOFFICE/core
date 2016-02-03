#pragma once

#include "BiffStructure.h"
#include "LongRGB.h"

namespace XLS
{

class CFRecord;

class GradStop : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(GradStop)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeGradStop;

	_UINT16		xclrType;
	_UINT32		xclrValue;
	LongRGBA	xclrValue_rgb;
	
	double		numPosition;
	double		numTint;
};

} // namespace XLS
