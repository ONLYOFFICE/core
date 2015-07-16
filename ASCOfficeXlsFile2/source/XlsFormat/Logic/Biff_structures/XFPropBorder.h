#pragma once

#include "BiffStructure.h"
#include "XFPropColor.h"

namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class XFPropBorder : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFPropBorder)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XFPropColor color;
	unsigned short dgBorder;
};
//#pragma pack() // restore

} // namespace XLS
