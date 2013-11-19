#pragma once

#include "BiffStructure.h"
#include "XFPropColor.h"

namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class XFPropBorder : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFPropBorder)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XFPropColor color;
	WORD dgBorder;
};
//#pragma pack() // restore

} // namespace XLS
