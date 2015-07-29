#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class SheetId : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SheetId)
public:
	BiffStructurePtr clone();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned short id;
};

typedef boost::shared_ptr<SheetId> SheetIdPtr;

} // namespace XLS

