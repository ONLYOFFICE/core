#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/XFPropGradient.h>
#include <XLS_logic/Biff_structures/GradStop.h>

namespace XLS
{;

class CFRecord;

//#pragma pack(1)
class XFExtGradient : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFExtGradient)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XFPropGradient gradient;
	DWORD cGradStops;
	std::vector<GradStop> rgGradStops;
};
//#pragma pack() // restore

} // namespace XLS
