#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/LongRGB.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

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

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
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

