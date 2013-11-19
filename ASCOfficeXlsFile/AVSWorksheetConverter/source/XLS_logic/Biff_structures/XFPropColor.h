#pragma once

#include "BiffStructure.h"
#include "LongRGB.h"

namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class XFPropColor : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFPropColor)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool fValidRGBA;
	BYTE xclrType;
	BYTE icv;
	short nTintShade;
	LongRGBA dwRgba;
};
//#pragma pack() // restore

} // namespace XLS
