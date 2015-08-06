#pragma once

#include "BiffStructure.h"
#include "FontIndex.h"

namespace XLS
{;

class CFRecord;

class FormatRun : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FormatRun)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFormatRun;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned short	ich;
	FontIndex		ifnt;
};

typedef boost::shared_ptr<FormatRun> FormatRunPtr;

} // namespace XLS
