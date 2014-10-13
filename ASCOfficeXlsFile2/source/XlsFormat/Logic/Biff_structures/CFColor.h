#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/LongRGB.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class CFColor : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CFColor)
public:
	CFColor();
	CFColor(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XColorType xclrType;
	ColorICV icv;
	LongRGBA rgb;
	ColorTheme theme;
	double numTint;
};

} // namespace XLS

