#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class MDir : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MDir)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeMDir;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	long imdt;
	unsigned short mdd;
};

typedef boost::shared_ptr<MDir> MDirPtr;

} // namespace XLS
