#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class ISSTInf : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ISSTInf)
public:
	BiffStructurePtr clone();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned int ib;
	unsigned short cbOffset;
};

typedef boost::shared_ptr<ISSTInf> ISSTInfPtr;

} // namespace XLS

