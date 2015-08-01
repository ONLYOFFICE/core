#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/LongRGB.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class CFColor : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFColor)
public:
	CFColor();
	CFColor(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeCFColor;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	XColorType xclrType;
	ColorICV icv;
	LongRGBA rgb;
	ColorTheme theme;
	double numTint;
};

} // namespace XLS

