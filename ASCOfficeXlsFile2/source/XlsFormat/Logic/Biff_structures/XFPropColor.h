#pragma once

#include "BiffStructure.h"
#include "LongRGB.h"

namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class XFPropColor : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFPropColor)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fValidRGBA;
	unsigned char xclrType;
	unsigned char icv;
	short nTintShade;
	LongRGBA dwRgba;
};
//#pragma pack() // restore

} // namespace XLS
