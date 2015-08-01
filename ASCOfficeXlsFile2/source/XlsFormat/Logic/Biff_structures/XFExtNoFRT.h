#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/ExtProp.h>

namespace XLS
{;

class CFRecord;

class XFExtNoFRT : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFExtNoFRT)
public:
	XFExtNoFRT();
	XFExtNoFRT(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeXFExtNoFRT;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	std::vector<ExtProp> rgExt;
};

typedef boost::shared_ptr<XFExtNoFRT> XFExtNoFRTPtr;

} // namespace XLS

