#pragma once

#include "BiffStructure.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtGboData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtGboData)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeFtGboData;

	unsigned short accel;
	bool fNo3d;
};

} // namespace XLS

