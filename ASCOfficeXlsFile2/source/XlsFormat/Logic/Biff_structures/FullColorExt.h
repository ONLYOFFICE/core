#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FullColorExt : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FullColorExt)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char	icv;
	unsigned char	xclrType;
	short			nTintShade;
	unsigned int	xclrValue;
};

} // namespace XLS
