#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/FullColorExt.h>
#include <Logic/Biff_structures/XFExtGradient.h>

namespace XLS
{

class CFRecord;

class XFProp : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFProp)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeXFProp;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short xfPropType;
	unsigned short cb;
	BiffStructurePtr xfPropDataBlob;
};

} // namespace XLS
