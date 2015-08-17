#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{

class CFRecord;

class HorzBrk : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(HorzBrk)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeHorzBrk;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	RwU row;
	unsigned short colStart;
	unsigned short colEnd;
};

typedef boost::shared_ptr<HorzBrk> HorzBrkPtr;

} // namespace XLS

