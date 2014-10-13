#pragma once

#include "BiffStructure.h"
#include "XFPropColor.h"

namespace XLS
{;

class CFRecord;

class XFPropGradientStop : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFPropGradientStop)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	double numPosition;
	XFPropColor color;
};

} // namespace XLS
