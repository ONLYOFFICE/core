#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FileOffset : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FileOffset)
public:
	BiffStructurePtr clone();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned short offset;
};

typedef boost::shared_ptr<FileOffset> FileOffsetPtr;

} // namespace XLS

