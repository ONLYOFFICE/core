#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;

class FilePointer : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FilePointer)
public:
	FilePointer();
	FilePointer(const unsigned int ptr);

	BiffStructurePtr clone();

	static const ElementType	type = typeFilePointer;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned int offset;
};

typedef boost::shared_ptr<FilePointer> FilePointerPtr;

} // namespace XLS

