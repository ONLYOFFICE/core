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

	static const ElementType	type = typeFileOffset;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short offset;
};

typedef boost::shared_ptr<FileOffset> FileOffsetPtr;

} // namespace XLS

