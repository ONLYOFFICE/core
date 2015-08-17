#pragma once

#include "BiffStructure.h"
#include "BiffString.h"

namespace XLS
{;

class CFRecord;

class DXFNumUsr : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFNumUsr)
public:
	DXFNumUsr();
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFNumUsr;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short cb;
	XLUnicodeString fmt;
};

} // namespace XLS

