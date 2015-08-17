#pragma once

#include "BiffStructure.h"
#include "Boolean.h"

namespace XLS
{

class CFRecord;

class FtRboData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtRboData)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFtRboData;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short idRadNext;
	Boolean<unsigned short> fFirstBtn;
};

} // namespace XLS

