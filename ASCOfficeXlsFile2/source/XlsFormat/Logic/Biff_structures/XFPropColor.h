#pragma once

#include "BiffStructure.h"
#include "LongRGB.h"

namespace XLS
{;

class CFRecord;

class XFPropColor : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFPropColor)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeXFPropColor;

	bool fValidRGBA;
	unsigned char xclrType;
	unsigned char icv;
	short nTintShade;
	LongRGBA dwRgba;
};

} // namespace XLS
