#pragma once

#include "BiffStructure.h"
#include "LongRGB.h"

namespace XLS
{;

class CFRecord;

class GradStop : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GradStop)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD xclrType;
	DWORD xclrValue;
	LongRGBA xclrValue_rgb;
	double numPosition;
	double numTint;
};

} // namespace XLS
