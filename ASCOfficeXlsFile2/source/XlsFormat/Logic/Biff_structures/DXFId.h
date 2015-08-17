#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class DXFId : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFId)
public:
	DXFId();
	DXFId(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFId;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned int index;
};

} // namespace XLS

