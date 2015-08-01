#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class VertBrk : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(VertBrk)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeVertBrk;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	ColU col;
	RwU rowStart;
	RwU rowEnd;
};

typedef boost::shared_ptr<VertBrk> VertBrkPtr;

} // namespace XLS

