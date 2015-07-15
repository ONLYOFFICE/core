#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/FullColorExt.h>
#include <Logic/Biff_structures/XFExtGradient.h>

namespace XLS
{;

class CFRecord;

class ExtProp : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExtProp)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 extType;
	unsigned __int16 cb;
	struct extPropData_Tag
	{
		FullColorExt color;
		XFExtGradient gradient_fill;
		unsigned char font_scheme;
		unsigned __int16 indent_level;

	} extPropData;
};

typedef boost::shared_ptr<ExtProp> ExtPropPtr;

} // namespace XLS
