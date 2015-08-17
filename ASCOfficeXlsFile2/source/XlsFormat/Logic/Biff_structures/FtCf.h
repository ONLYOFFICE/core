#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;

class FtCf : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtCf)
public:
	FtCf() 
	{
		cf = 0xFFFF;
	}

	BiffStructurePtr clone();
	static const ElementType	type = typeFtCf;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short cf;
};

} // namespace XLS

