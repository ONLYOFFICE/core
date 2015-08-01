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

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeExtProp;

	unsigned short extType;
	unsigned short cb;
	
	struct extPropData_Tag
	{
		FullColorExt color;
		XFExtGradient gradient_fill;
		unsigned char font_scheme;
		unsigned short indent_level;

	} extPropData;
};

typedef boost::shared_ptr<ExtProp> ExtPropPtr;

} // namespace XLS
