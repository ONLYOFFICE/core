#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;

class MDXStrIndex : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MDXStrIndex)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeMDXStrIndex;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned int index;
};

typedef boost::shared_ptr<MDXStrIndex> MDXStrIndexPtr;

} // namespace XLS

