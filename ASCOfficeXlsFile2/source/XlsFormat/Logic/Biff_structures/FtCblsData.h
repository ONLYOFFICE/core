#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtCblsData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtCblsData)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFtCblsData;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short fChecked;
	unsigned short accel;
	bool fNo3d;
};

} // namespace XLS

