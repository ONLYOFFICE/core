#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/XFProp.h>

namespace XLS
{;

class CFRecord;

class XFProps : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFProps)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short cprops;
	std::vector<XFProp> xfPropArray;
};

} // namespace XLS
