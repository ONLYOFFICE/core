#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/XFPropGradient.h>
#include <Logic/Biff_structures/GradStop.h>

namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class XFExtGradient : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFExtGradient)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XFPropGradient gradient;
	unsigned int cGradStops;
	std::vector<GradStop> rgGradStops;
};
//#pragma pack() // restore

} // namespace XLS
