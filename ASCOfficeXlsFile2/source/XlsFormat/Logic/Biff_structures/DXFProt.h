#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class DXFProt : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFProt)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFProt;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fLocked;
	bool fHidden;
};

} // namespace XLS

