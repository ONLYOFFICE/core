#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class DXFPat : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFPat)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFPat;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	FillPattern fls;
	unsigned char icvForeground;
	unsigned char icvBackground;
};

} // namespace XLS

